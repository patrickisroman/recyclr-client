#include "recyclr-net.h"

#include <unistd.h>
#include <sched.h>
#include <algorithm>

//////////////// VerticalNetClient ////////////////
VerticalNetClient::VerticalNetClient() :
    _thr(nullptr),
    _fd(-1),
    _running(false),
    _state_fn(nullptr)
{
    _running = true;
    _thr = new std::thread(&VerticalNetClient::run, this);

    int r = set_thread_affinity(_thr, NETWORK_THREAD_MASK);
    if (r) {
        log("Unable to set thread affinity for vertical network thread");
    }
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

    while(_state_fn && _running) {
        if ((this->*_state_fn)()) {
            break;
        }
    }

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

u32 VerticalNetClient::handshake()
{
    return 0;
}

u32 VerticalNetClient::disconnect()
{
    return 0;
}

//////////////// HorizontalNetClient ////////////////
HorizontalNetClient::HorizontalNetClient() :
    _thr(nullptr),
    _fds(),
    _running(false),
    _state_fn(nullptr)
{
    _running = true;
    _thr = new std::thread(&HorizontalNetClient::run, this);

    int r = set_thread_affinity(_thr, NETWORK_THREAD_MASK);
    if (r) {
        log("Unable to set thread affinity for horizontal network thread");
    }
}

HorizontalNetClient::~HorizontalNetClient()
{
    while (!_fds.empty()) {
        int fd = _fds.back();
        if (::close(fd)) {
            int err = errno;
        }
        _fds.pop_back();
    }

    if (_thr) {
        _running = false;
        _thr->join();
        _thr = nullptr;
    }
}

u32 HorizontalNetClient::run()
{
    if (!_state_fn) {
        _state_fn = &HorizontalNetClient::listen;
    }

    while(_state_fn && _running) {
        if ((this->*_state_fn)()) {
            break;
        }
    }

    return 0;
}

u32 HorizontalNetClient::listen()
{
    sched_yield();

    if (!_running) {
        return -1;
    }

    return 0;
}

u32 HorizontalNetClient::handshake()
{
    return 0;
}

u32 HorizontalNetClient::disconnect()
{
    return 0;
}