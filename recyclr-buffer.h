#pragma once

#include <algorithm>
#include <cstring>

class RingBuffer
{
    protected:
    size_t _start_idx;
    size_t _end_idx;
    size_t _size;
    size_t _capacity;
    char*  _ring_buffer;

    public:
    RingBuffer(size_t capacity);
    ~RingBuffer();

    size_t get_size() const
    {
        return _size;
    }

    size_t get_capacity() const
    {
        return _capacity;
    }

    size_t write(const char* buffer, size_t len);
    size_t read(char* buffer, size_t len);
    size_t read_all(char* buffer, size_t buffer_len);
};