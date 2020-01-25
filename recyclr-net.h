#include "recyclr-utils.h"

#include <string>
#include <vector>
#include <sys/epoll.h>
#include <deque>

#define RECYCLR_NETWORK_PORT    6528
#define RECYCLR_MAX_CONNECTIONS 300
#define RECYCLR_BACKLOG         32
#define RECYCLR_MAX_FDS         16 * 1024
#define BLOB_HEADER_LEN_BYTES   1 << 6

using namespace std;
using buffer_queue = deque<pair<char*, size_t>>;

enum BlobOpCode
{
    OP_CODE_OPEN_CONNECTION  = 0,
    OP_CODE_CLOSE_CONNECTION = 1,
    OP_CODE_REQUEST_CALLBACK = 2,
    OP_CODE_PUSH_DATA = 3,
    OP_CODE_RECALL = 4
};

#define BLOB_MAGIC 0x83702020

struct blob_header {
    BlobOpCode op_code; 
    u32        magic;
    u64        message_length_bytes;
    u64        source_id;
    u64        target_id;
    u64        message_id;
};

struct local_buffer {
    void*  buffer;
    size_t len;
};

struct NetworkBlobHeader
{
    union {
        unsigned char      buffer[BLOB_HEADER_LEN_BYTES];
        struct blob_header header_data;
    };
};

static_assert(sizeof(NetworkBlobHeader) == BLOB_HEADER_LEN_BYTES);

class NetworkBlob
{
    protected:
    NetworkBlobHeader _header;

    private:
    local_buffer _blob_buffer;
    local_buffer _payload_buffer;

    public:
    NetworkBlob();
    ~NetworkBlob();

    local_buffer* from_buffer(void* buffer, size_t len);
    local_buffer* load_buffer();
    
    bool set_payload(void* data, size_t len);
    bool append_payload(void* data, size_t len);
};

// TODO Make a parent template class for Client
// ugh that's gonna be whacky function ptr logic

class NetClient
{
    protected:
    thread*             _thr;
    int                 _socket_fd;
    int                 _epoll_fd;
    struct epoll_event* _epoll_events;
    bool                _running;
    buffer_queue        _buffer_queue;

    u32 (NetClient::*_state_fn)();

    public:
    NetClient();
    ~NetClient();

    u32 run();
    u32 listen();
    u32 handle_socket();
    u32 process_blobs();
    u32 close();

    bool pop_message(local_buffer* buffer);
    void stop();
};

int setup_listening_socket();
int accept_connection(int socket);
int set_socket_flags(int socket_fd, int flags);
int handle_epoll_event(struct epoll_event& event, buffer_queue& queue);