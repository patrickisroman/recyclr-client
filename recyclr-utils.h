#pragma once

#include <iostream>
#include <sys/time.h>
#include <thread>

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

template<typename ...ArgType>
void log(ArgType && ...args)
{
    (std::cout << "[" << micros() << "] [" << sched_getcpu() << "] ");
    (std::cout << ... << args);
    (std::cout << "\n");
}

template<typename ...ArgType>
void err(ArgType && ...args)
{
    (std::cout << "[" << micros() << "] " << sched_getcpu() << "] ");
    std::cout << "[ERROR]";
    (std::cout << ... << args);
    std::cout << "\n";
}

#define ATOMIC_ADD(ptr, val) \
    __sync_add_and_fetch(ptr, val)

#define ATOMIC_SUB(ptr, val) \
    __sync_sub_and_fetch(ptr, val)

#define ATOMIC_CAS(ptr, old_val, new_val) \
    __sync_bool_compare_and_swap(ptr, old_val, new_val)

// Threading functions
u32 set_thread_affinity(std::thread* thr, thread_mask affinity);
u32 set_process_affinity(thread_mask affinity);
