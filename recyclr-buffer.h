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
    __attribute__((always_inline))
    T* peek(T* dest)
    {
        if (!dest || get_size() < sizeof(T)) return nullptr;

        size_t start_bytes = get_size() - _start_idx;
        if (_end_idx < _start_idx) {
            memcpy(dest, _ring_buffer + _start_idx, start_bytes);
            memcpy(dest + start_bytes, _ring_buffer + start_bytes, sizeof(T) - start_bytes);
        } else {
            memcpy(dest, _ring_buffer + _start_idx, sizeof(T));
        }

        return dest;
    }

    template<typename T>
    __attribute__((always_inline))
    T* pop(T* dest)
    {
        if (!dest || _size < sizeof(T)) return nullptr;
        read(reinterpret_cast<char*>(dest), sizeof(T));
        return dest;
    }

    char* pop_buf(char* dest, size_t len)
    {
        if (!dest || get_size() < len) return nullptr;
        read(dest, len);
        return dest;
    }

    size_t write(const char* buffer, size_t len);
    size_t read(char* buffer, size_t len);
    size_t read_all(char* buffer, size_t buffer_len);
};