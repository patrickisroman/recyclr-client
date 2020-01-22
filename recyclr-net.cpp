#include "recyclr-net.h"

#include <unistd.h>
#include <sched.h>
#include <algorithm>

//////////////// NetworkBlob //////////////////

NetworkBlob::NetworkBlob() :
    _header(),
    _blob_buffer(),
    _payload_buffer()
{
}

local_buffer* NetworkBlob::to_buffer()
{
    size_t blob_buffer_size = sizeof(NetworkBlobHeader) + _payload_buffer.len;

    // We have an allocated buffer, but it's the wrong size. Deallocate.
    if (_blob_buffer.buffer && _blob_buffer.len != blob_buffer_size) {
        free(_blob_buffer.buffer);
        _blob_buffer = {nullptr, 0};
    }

    // We don't have a blob buffer. Allocate.
    if (!_blob_buffer.buffer) {
        _blob_buffer.buffer = malloc(blob_buffer_size);

        if (!_blob_buffer.buffer) {
            _blob_buffer.len = 0;
            log("ERROR: Unable to allocate blob message buffer");
            return nullptr;
        }
    }

    unsigned char* byte_buffer = reinterpret_cast<unsigned char*>(_blob_buffer.buffer);

    // Copy header
    *(reinterpret_cast<NetworkBlobHeader*>(byte_buffer)) = _header;
    byte_buffer += sizeof(NetworkBlobHeader);

    // Copy payload
    memcpy(byte_buffer, reinterpret_cast<unsigned char*>(_payload_buffer.buffer), _payload_buffer.len);
    return &_blob_buffer;
}

NetworkBlob::~NetworkBlob()
{
    // clean up the payload buffer
    if (_payload_buffer.buffer) {
        free(_payload_buffer.buffer);
        _payload_buffer = {nullptr, 0};
    }

    // clean up the blob buffer
    if (_blob_buffer.buffer) {
        free(_blob_buffer.buffer);
        _blob_buffer = {nullptr, 0};
    }
}

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