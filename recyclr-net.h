#include "recyclr-utils.h"

#include <thread>
#include <string>
#include <vector>

#define BASE_SYNC_PORT   5440
#define CLIENT_SYNC_PORT 5441

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