#include "recyclr-types.h"

#include <thread>
#include <string>

#define BASE_SYNC_PORT   5440
#define CLIENT_SYNC_PORT 5441

class NetClient
{
    protected:
    u32  handshake();
    u32  listen();
    u32  disconnect();

    public:
    NetClient() {};
    ~NetClient() {};
};

class VerticalNetClient : public NetClient
{
    protected:
    std::thread* _thr;
    int          _fd;

    u32 handshake();
    u32 listen();
    u32 disconnect();

    public:
    VerticalNetClient();
    ~VerticalNetClient();
};