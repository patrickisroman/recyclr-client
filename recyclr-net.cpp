#include "recyclr-net.h"

#include <unistd.h>
#include <sched.h>
#include <algorithm>
#include <string.h>

//////////////// NetworkBlob //////////////////

NetworkBlob::NetworkBlob() :
    _header(),
    _blob_buffer(),
    _payload_buffer()
{
}

local_buffer* NetworkBlob::from_buffer(void* buffer, size_t len)
{
    if (!buffer) {
        return &_blob_buffer;
    }

    if (len != _blob_buffer.len) {
        void* ptr = malloc(len);
        if (!ptr) {
            log("ERROR: Unable to realloc buffer to load blob from buffer");
            return nullptr;
        }

        if (_blob_buffer.buffer) {
            free(_blob_buffer.buffer);
        }

        _blob_buffer = {ptr, len};
    }

    memcpy(_blob_buffer.buffer, buffer, len);
    return &_blob_buffer;
}

local_buffer* NetworkBlob::load_buffer()
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

    char* blob_buffer_ptr = reinterpret_cast<char*>(_blob_buffer.buffer);

    // Copy header
    *(reinterpret_cast<NetworkBlobHeader*>(blob_buffer_ptr)) = _header;
    blob_buffer_ptr += sizeof(NetworkBlobHeader);

    // Copy payload
    memcpy(blob_buffer_ptr, _payload_buffer.buffer, _payload_buffer.len);
    return &_blob_buffer;
}

bool NetworkBlob::set_payload(void* data, size_t len)
{
    if (!data) {
        return false;
    }

    if (!len) {
        return true;
    }

    // Payload already exists, but it's the wrong size. Reallocate
    if (_payload_buffer.buffer && _payload_buffer.len != len) {
        void* payload_buffer_ptr = malloc(len);
        if (!payload_buffer_ptr) {
            log("ERROR: Unable to allocate payload buffer");
            return false;
        }

        free(_payload_buffer.buffer);
        _payload_buffer = {payload_buffer_ptr, len};
    }

    // No payload exists. Allocate.
    if (!_payload_buffer.buffer) {
        void* payload_buffer_ptr = malloc(len);
        if (!payload_buffer_ptr) {
            log("ERROR: Unable to allocate payload buffer");
            return false;
        }
        _payload_buffer = {payload_buffer_ptr, len};
    }

    memcpy(_payload_buffer.buffer, data, len);

    return true;
}

bool NetworkBlob::append_payload(void* data, size_t len)
{
    if (!data) {
        return false;
    }

    if (!len) {
        return true;
    }

    if (!_payload_buffer.buffer) {
        return set_payload(data, len);
    }

    void* payload_copy_buffer = malloc(_payload_buffer.len + len);
    char* payload_copy_ptr = reinterpret_cast<char*>(payload_copy_buffer);

    memcpy(payload_copy_ptr, _payload_buffer.buffer, _payload_buffer.len);
    memcpy(payload_copy_ptr + _payload_buffer.len, data, len);

    // This is NOT thread safe; thus why we must pin Networking to a single core
    free(_payload_buffer.buffer);
    _payload_buffer.buffer = payload_copy_ptr;

    return true;
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