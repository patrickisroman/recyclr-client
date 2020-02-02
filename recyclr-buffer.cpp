#include "recyclr-buffer.h"

RingBuffer::RingBuffer(size_t capacity) :
    _start_idx(0),
    _end_idx(0),
    _size(0),
    _capacity(capacity)
{
    _ring_buffer = new char[capacity];
}

RingBuffer::~RingBuffer()
{
    delete[] _ring_buffer;
}

size_t RingBuffer::write(const char* buffer, size_t len)
{
    if (len == 0) {
        return 0;
    }

    size_t write_len = std::min(len, _capacity - _size);
    if (write_len == 0) {
        return 0;
    }

    if (write_len <= _capacity - _end_idx) {
        memcpy(_ring_buffer + _end_idx, buffer, write_len);
        _end_idx += write_len;
        if (_end_idx == _capacity) {
            _end_idx = 0;
        }
    } else {
        size_t end_write_len = _capacity - _end_idx;
        size_t wrap_write_len = write_len - end_write_len;
        memcpy(_ring_buffer + _end_idx, buffer, end_write_len);
        memcpy(_ring_buffer, buffer + end_write_len, wrap_write_len);
        _end_idx = wrap_write_len;
    }

    _size += write_len;
    return write_len;
}

size_t RingBuffer::read(char* data, size_t len)
{
    if (len == 0) {
        return 0;
    }

    size_t read_len = std::min(len, _size);
    if (read_len == 0) {
        return 0;
    }

    if (read_len <= _capacity - _start_idx) {
        memcpy(data, _ring_buffer + _start_idx, read_len);
        _start_idx += read_len;
        if (_start_idx == _capacity) {
            _start_idx = 0;
        }
    } else {
        size_t end_read_len = _capacity - _start_idx;
        size_t wrap_read_len = read_len - end_read_len;
        memcpy(data, _ring_buffer + _start_idx, end_read_len);
        memcpy(data + end_read_len, _ring_buffer, wrap_read_len);
        _start_idx = wrap_read_len;
    }

    _size -= read_len;
    
    // reset to head if possible
    // we only read from a single core so no data races
    if (_size == 0) {
        _start_idx = 0;
        _end_idx = 0;
    }

    return read_len;
}

size_t RingBuffer::read_all(char* data, size_t buffer_len)
{
    if (buffer_len < _size) {
        return 0;
    }

    return read(data, _size);
}