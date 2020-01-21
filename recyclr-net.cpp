#include "recyclr-net.h"

#include <unistd.h>
#include <sched.h>

VerticalNetClient::VerticalNetClient() :
    _thr(nullptr),
    _fd(-1),
    _running(false),
    _state_fn(nullptr)
{
    _running = true;
    _thr = new std::thread(&VerticalNetClient::run, this);
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
    if (!_state_fn) {
        _state_fn = &VerticalNetClient::listen;
    }

    while(_state_fn) {
        if ((this->*_state_fn)()) {
            break;
        }
    }

    return 0;
}

u32 VerticalNetClient::handshake()
{
    return 0;
}

u32 VerticalNetClient::listen()
{
    sched_yield();

    if (!_running) {
        return -1;
    }

    return 0;
}

u32 VerticalNetClient::disconnect()
{
    return 0;
}