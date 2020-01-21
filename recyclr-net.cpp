#include "recyclr-net.h"

#include <iostream>
#include <unistd.h>

VerticalNetClient::VerticalNetClient() :
    _thr(nullptr),
    _fd(-1),
    _running(false)
{
    _running = true;
    _thr = new std::thread(&VerticalNetClient::run, this);
    _thr->join();
}

VerticalNetClient::~VerticalNetClient()
{
    if (_fd) {
        if (::close(_fd)) {
            int err = errno;
        }
        _fd = 0;
    }

    if (_thr) {
        _running = false;
        _thr->join();
        _thr = nullptr;
    }
}

u32 VerticalNetClient::run()
{
    while(_running) {
        std::cout << "a\n";
    }

    return 0;
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