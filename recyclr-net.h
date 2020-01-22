#include "recyclr-utils.h"

#include <string>
#include <vector>

#define RECYCLR_NETWORK_PORT  6528

#define BLOB_HEADER_LEN_BYTES 1 << 9

enum BlobOperation
{
    OPEN_CONNECTION  = 0,
    CLOSE_CONNECTION = 1,
    REQUEST_CALLBACK = 2,
    PUSH_DATA = 3,
    RECALL = 4
};

struct blob_header {
    BlobOperation operation;
    u64           source_id;
    u64           target_id;
    u64           message_id;
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

    local_buffer* to_buffer();
    
    bool set_payload(void* data, size_t len);
    bool append_payload(void* data, size_t len);
};

// TODO Make a parent template class for Client
// ugh that's gonna be whacky function ptr logic

class VerticalNetClient
{
    protected:
    std::thread* _thr;
    int          _fd;
    bool         _running;

    u32 (VerticalNetClient::*_state_fn)();

    public:
    VerticalNetClient();
    ~VerticalNetClient();


    u32 listen();
    u32 handshake();
    u32 disconnect();
    u32 run();
};

class HorizontalNetClient
{
    protected:
    std::thread*     _thr;
    std::vector<int> _fds;
    bool             _running;

    u32 (HorizontalNetClient::*_state_fn)();

    public:
    HorizontalNetClient();
    ~HorizontalNetClient();

    u32 listen();
    u32 handshake();
    u32 disconnect();
    u32 run();
};