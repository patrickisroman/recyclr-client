#pragma once

#include <algorithm>
#include <cstring>

// Yo this ain't threadsafe.
// It's ok for now, but we should invest in thread-safe buffers!
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

    size_t get_space() const
    {
        return _capacity - _size;
    }

    template<typename T>
    T* peek()
    {
        return reinterpret_cast<T*>(_ring_buffer + _start_idx);
    }

    template<typename T>
    T* pop(T* dest)
    {
        if (!dest || get_size() < sizeof(T)) return nullptr;
        read(reinterpret_cast<char*>(dest), sizeof(T));
        return dest;
    }

    size_t write(const char* buffer, size_t len);
    size_t read(char* buffer, size_t len);
    size_t read_all(char* buffer, size_t buffer_len);
};