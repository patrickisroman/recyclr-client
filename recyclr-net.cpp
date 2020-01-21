#include "recyclr-net.h"

#include <unistd.h>

VerticalNetClient::VerticalNetClient() :
    _thr(nullptr),
    _fd(-1)
{
}

VerticalNetClient::~VerticalNetClient()
{
    if (_fd) {
        if (::close(_fd)) {
            int err = errno;
        }
        _fd = 0;
    }
}

u32 VerticalNetClient::handshake()
{
    return 0;
}

u32 VerticalNetClient::listen()
{
    return 0;
}

u32 VerticalNetClient::disconnect()
{
    return 0;
}