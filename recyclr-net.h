#include "recyclr-utils.h"
#include "recyclr-buffer.h"
#include "recyclr-msg.h"

#include <string>
#include <vector>
#include <sys/epoll.h>
#include <deque>

#define RECYCLR_NETWORK_PORT    6528
#define RECYCLR_MAX_CONNECTIONS 300
#define RECYCLR_BACKLOG         32
#define RECYCLR_MAX_FDS         16 * 1024
#define MSG_HEADER_LEN_BYTES    1 << 6
#define CONNECTION_BUFFER_LEN   1 << 20

class NetClient;

class Connection {
    friend class NetClient;

    protected:
    RingBuffer                _in_buffer;
    RingBuffer                _out_buffer;
    int                       _fd;
    char                      _peer_ipv4_addr[32];
    struct recyclr_msg_header _reusable_header;
    u64                       _last_send_time;

    u32 (Connection::*_state_fn)();

    public:
    Connection(int _fd = -1, int buffer_size = CONNECTION_BUFFER_LEN);
    ~Connection();

    int get_fd() const
    {
        return _fd;
    }

    char* get_peer_ipv4_addr() const
    {
        return const_cast<char*>(_peer_ipv4_addr);
    }

    RingBuffer& get_in_buffer()
    {
        return _in_buffer;
    }

    size_t recv();
    void   send();
    void   prepare_send(void* buffer, size_t len, bool immediate=false);

    u32    await_handshake();
    u32    open_connection();
    u32    handle_open_channel();
    u32    close();
    u32    state();
};

class NetClient
{
    friend class Connection;

    protected:
    std::thread*        _thr;
    int                 _socket_fd;
    int                 _epoll_fd;
    struct epoll_event* _epoll_events;
    bool                _running;

    static std::vector<Connection*> _open_connections;

    u32 (NetClient::*_state_fn)();

    public:
    NetClient();
    ~NetClient();

    u32 run();
    u32 start();
    u32 listen();
    u32 handle_socket();
    u32 process_connections();
    u32 close();

    void stop();
    int handle_epoll_event(struct epoll_event& event, Connection* conn);
};

int setup_listening_socket();
int accept_connection(int socket);
int set_socket_flags(int socket_fd, int flags);