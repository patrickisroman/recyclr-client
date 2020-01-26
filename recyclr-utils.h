#pragma once

#include <iostream>
#include <sys/time.h>
#include <thread>
#include <iomanip>
#include <mutex>

#include "recyclr-types.h"

#define MILLIS_PER_SECOND 1000
#define MICROS_PER_MILLI  1000
#define MICROS_PER_SECOND 1000000
#define NANOS_PER_SECOND  1000000000

enum thread_mask {
    BASE_THREAD_MASK       = 0x0,
    NETWORK_THREAD_MASK    = 0x1,
    IMAGE_THREAD_MASK      = 0x2,
    CONTROLLER_THREAD_MASK = 0x3
};

// Time functions
u64 micros();
u64 millis();

#define LOG(...) log("[LOG] ", __LINE__, __FILE__, __VA_ARGS__)
#define ERR(...) log("[ERROR] ", __LINE__, __FILE__, __VA_ARGS__);

template<typename ...ArgType>
void log(const char* prefix, int line, const char* file, ArgType && ...args)
{
    double time = ((double) micros()) / MICROS_PER_SECOND;
    std::cout << std::fixed;
    std::cout << "[" << time << "] (" << sched_getcpu() << ") [" << file << ":" << line << "] ";
    (std::cout << ... << args);
    std::cout << "\n";
}

#define ATOMIC_ADD(ptr, val) \
    __sync_add_and_fetch(ptr, val)

#define ATOMIC_SUB(ptr, val) \
    __sync_sub_and_fetch(ptr, val)

#define ATOMIC_CAS(ptr, old_val, new_val) \
    __sync_val_compare_and_swap(ptr, old_val, new_val)

#define ATOMIC_CAS_BOOL(ptr, old_val, new_val) \
    __sync_bool_compare_and_swap(ptr, old_val, new_val)

// Threading functions
u32 set_thread_affinity(std::thread* thr, thread_mask affinity);
u32 set_process_affinity(thread_mask affinity);
