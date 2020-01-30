#include "recyclr-net.h"

#include <unistd.h>
#include <sched.h>
#include <algorithm>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

/////////////// Connection ////////////////
Connection::Connection(int fd, int buffer_size) :
    _fd(fd),
    _in_buffer(buffer_size),
    _out_buffer(buffer_size)
{
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);
    int r = getpeername(fd, (struct sockaddr*)&addr, &addr_size);
    strcpy(_peer_ipv4_addr, inet_ntoa(addr.sin_addr));
}

Connection::~Connection()
{
    if (_fd >= 0) {
        int r = ::close(_fd);
        if (r < 0) {
            int err = errno;
            if (err == EIO) {
                ERR("Unable to close socket fd ", _fd);
            }
        }
    }
}

size_t Connection::recv()
{
    size_t read_length = 0;
    char big_buffer[1 << 20]; // 1MB lmaoo. lets find a way to nix this allocation
    int r;
    while ((r = ::recv(_fd, big_buffer + read_length, sizeof(big_buffer) - read_length, MSG_DONTWAIT)) > 0) {
        read_length += r;
    }

    if (read_length > 0) {
        _in_buffer.write(big_buffer, read_length);
    }

    if (r == 0) {
        LOG("Closing connection, fd: ", _fd);
        delete this;
        return 0;
    }

    return read_length;
}

void Connection::send()
{
    if (_out_buffer.get_size() == 0) {
        return;
    }
    char big_buffer[1 << 20];
    size_t bytes_remaining = _out_buffer.read_all(big_buffer, sizeof(big_buffer));
    size_t bytes_sent = 0;

    int r;
    while ((r = ::send(_fd, big_buffer + bytes_sent, bytes_remaining, MSG_DONTWAIT)) > 0) {
        bytes_remaining -= r;
        bytes_sent += r;
    }
}

void Connection::prepare_send(const char* buffer, size_t len)
{
    _out_buffer.write(buffer, len);
}

//////////////// NetClient ////////////////
NetClient::NetClient() :
    _thr(nullptr),
    _socket_fd(-1),
    _epoll_fd(-1),
    _epoll_events(),
    _running(false),
    _state_fn(nullptr)
{
    _running = true;
    _thr = new std::thread(&NetClient::run, this);

    int r = set_thread_affinity(_thr, NETWORK_THREAD_MASK);
    if (r) {
        ERR("Unable to set thread affinity for client network thread");
    }
}

NetClient::~NetClient()
{
    if (_socket_fd >= 0) {
        if (::close(_socket_fd)) {
            int err = errno;
        }
        _socket_fd = -1;
    }

    if (_thr) {
        _running = false;
        _thr->join();
        delete _thr;
        _thr = nullptr;
    }

    if (_epoll_events) {
        delete _epoll_events;
        _epoll_events = nullptr;
    }
}

u32 NetClient::run()
{
    if (!_state_fn) {
        _state_fn = &NetClient::start;
    }

    while (_state_fn && _running) {
        if ((this->*_state_fn)()) {
            break;
        }
    }

    return 0;
}

u32 NetClient::start()
{
    _state_fn = &NetClient::listen;
    return 0;
}

u32 NetClient::listen()
{
    _socket_fd = setup_listening_socket();
    if (_socket_fd < 1) {
        _state_fn = &NetClient::close;
        return -1;
    }

    int r = set_socket_flags(_socket_fd, O_NONBLOCK);
    if (r < 0) {
        ERR("Unable to set socket flags. Socket: ", _socket_fd);
        _state_fn = &NetClient::close;
        return -1;
    }

    _epoll_fd = epoll_create(1);
    if (_epoll_fd < 0) {
        ERR("Unable to setup epoll listener");
        _state_fn = &NetClient::close;
        return -1;
    }

    struct epoll_event listening_socket_event;
    listening_socket_event.data.ptr = nullptr;
    listening_socket_event.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLET;
    r = epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _socket_fd, &listening_socket_event);
    if (r < 0) {
        ERR("Unable to register epoll events on listening socket");
        _state_fn = &NetClient::close;
        return -1;
    }

    _epoll_events = new struct epoll_event[RECYCLR_MAX_FDS];
    if (!_epoll_events) {
        ERR("Unable to allocate epoll event list");
        _state_fn = &NetClient::close;
        return -1;
    }

    _state_fn = &NetClient::handle_socket;
    return 0;
}

u32 NetClient::handle_socket()
{
    int events = epoll_wait(_epoll_fd, _epoll_events, RECYCLR_MAX_FDS, 0);
    for (int i = 0; i < events; i++) {
        struct epoll_event& fd_event = _epoll_events[i];

        Connection* conn = (Connection*) fd_event.data.ptr;
        if (fd_event.events & EPOLLERR && conn) {
            ERR("ERR ON SOCKET ", conn->get_fd());
        }

        if (conn == nullptr) {
            int conn_fd = accept_connection(_socket_fd);
            if (conn_fd < 0) {
                int e = errno;
                if (e != EWOULDBLOCK && e != EAGAIN) {
                    ERR("Fatal error while accepting a new connection, err: ", e);
                    _state_fn = &NetClient::close;
                    return -1;
                }
            } else {
                // add socket to epoll event
                int r = set_socket_flags(_socket_fd, O_NONBLOCK);
                if (r < 0) {
                    _state_fn = &NetClient::close;
                    return -1;
                }

                Connection* conn = new Connection(conn_fd, CONNECTION_BUFFER_LEN);
                if (!conn) {
                    ERR("Unable to allocate connection for new fd ", conn_fd);
                }

                struct epoll_event event;
                event.data.ptr = conn;
                event.events = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLET;
                r = epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, conn_fd, &event);
                if (r < 0) {
                    ERR("Unable to add new connection to epoll listener, err: ", r);
                    _state_fn = &NetClient::close;
                    return -1;
                }

                LOG("Accepted a connection from: ", conn->get_peer_ipv4_addr());
            }
        } else {
            handle_epoll_event(fd_event, conn);
        }
    }

    return 0;
}

u32 NetClient::process_msgs()
{
    _state_fn = &NetClient::handle_socket;
    return 0;
}

u32 NetClient::close()
{
    return 1;
}

void NetClient::stop()
{
    _running = false;
}

int NetClient::handle_epoll_event(struct epoll_event& event, Connection* conn)
{
    if (!conn) {
        return -1;
    }

    if (event.events & (EPOLLERR | EPOLLHUP)) {
        LOG("ERR Closing connection, fd: ", conn->get_fd());
        if (conn) {
            delete conn;
            return -1;
        }
    }

    // data received on socket
    if (event.events & EPOLLIN) {
        conn->recv();
    }

    // data was unable to send on socket, now available
    if (Event.events & EPOLLOUT) {
        conn->send();
    }

    return 0;
}

int setup_listening_socket()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        ERR("Unable to open socket fd");
        return -1;
    }

    int opt_name = 1;
    int r = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt_name, sizeof(opt_name));
    if (r < 0) {
        ERR("Unable to set socket as reusable");
        return -1;
    }

    timeval t;
    r = setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t));
    if (r < 0) {
        ERR("Unable to set immediate timeout for socket operations");
        return -1;
    }

    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(RECYCLR_NETWORK_PORT);
    bzero(&(local_addr.sin_zero), 8);
    r = bind(socket_fd, (struct sockaddr*)&local_addr, sizeof(local_addr));
    if (r < 0) {
        ERR("Unable to bind socket ", socket_fd);
        return -1;
    }

    r = listen(socket_fd, RECYCLR_BACKLOG);
    if (r < 0) {
        ERR("Unable to listen on socket ", socket_fd);
        return -1;
    }

    return socket_fd;
}

int accept_connection(int socket)
{
    struct sockaddr_in peer_address;
    socklen_t peer_addr_size = sizeof(struct sockaddr_in);
    return accept(socket, (struct sockaddr*)&peer_address, &peer_addr_size);
}

int set_socket_flags(int socket_fd, int flags)
{
    int socket_flags = fcntl(socket_fd, F_GETFL, 0);
    if (socket_flags < 0) {
        return socket_flags;
    }

    return fcntl(socket_fd, F_SETFL, socket_flags | flags);
}