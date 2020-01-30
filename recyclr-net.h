#include "recyclr-utils.h"
#include "recyclr-buffer.h"

#include <string>
#include <vector>
#include <sys/epoll.h>
#include <deque>

#define RECYCLR_NETWORK_PORT    6528
#define RECYCLR_MAX_CONNECTIONS 300
#define RECYCLR_BACKLOG         32
#define RECYCLR_MAX_FDS         16 * 1024
#define MSG_HEADER_LEN_BYTES    1 << 6
#define MSG_MAGIC               0x83702020
#define CONNECTION_BUFFER_LEN   1 << 20

enum msg_op_code : u32
{
    OP_CODE_OPEN_CONNECTION  = 0,
    OP_CODE_CLOSE_CONNECTION = 1,
    OP_CODE_REQUEST_CALLBACK = 2,
    OP_CODE_PUSH_DATA = 3,
    OP_CODE_RECALL = 4
};

struct msg_header {
    msg_op_code op_code; 
    u32         magic;
    u64         message_length_bytes;
    u64         source_id;
    u64         target_id;
    u64         message_id;
};

struct MsgHeader {
    union {
        struct msg_header header_data;
        unsigned char     buffer[MSG_HEADER_LEN_BYTES];
    };
};

static_assert(sizeof(MsgHeader) == MSG_HEADER_LEN_BYTES);

class NetClient;

class Connection {
    friend class NetClient;

    protected:
    int        _fd;
    RingBuffer _in_buffer;
    RingBuffer _out_buffer;
    char       _peer_ipv4_addr[32];

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
    void   prepare_send(const char* buffer, size_t len);
};

class NetClient
{
    protected:
    std::thread*        _thr;
    int                 _socket_fd;
    int                 _epoll_fd;
    struct epoll_event* _epoll_events;
    bool                _running;

    u32 (NetClient::*_state_fn)();

    public:
    NetClient();
    ~NetClient();

    u32 run();
    u32 start();
    u32 listen();
    u32 handle_socket();
    u32 process_msgs();
    u32 close();

    void stop();
    int handle_epoll_event(struct epoll_event& event, Connection* conn);
};

int setup_listening_socket();
int accept_connection(int socket);
int set_socket_flags(int socket_fd, int flags);