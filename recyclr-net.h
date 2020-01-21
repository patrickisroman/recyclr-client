#include "recyclr-types.h"

#include <thread>
#include <string>

#define BASE_SYNC_PORT   5440
#define CLIENT_SYNC_PORT 5441

class VerticalNetClient
{
    protected:
    std::thread* _thr;
    int          _fd;
    bool         _running;

    u32 handshake();
    u32 listen();
    u32 disconnect();
    u32 run();

    public:
    VerticalNetClient();
    ~VerticalNetClient();
};