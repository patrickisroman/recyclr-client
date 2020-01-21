#pragma once

#include <iostream>
#include <sys/time.h>

#include "recyclr-types.h"

#define MILLIS_PER_SECOND 1000
#define MICROS_PER_MILLI  1000
#define MICROS_PER_SECOND 1000000
#define NANOS_PER_SECOND  1000000000

template<typename ...ArgType>
void log(ArgType && ...args)
{
    (std::cout << ... << args);
    (std::cout << "\n");
}

#define ATOMIC_ADD(ptr, val) \
    __sync_add_and_fetch(ptr, val)

#define ATOMIC_SUB(ptr, val) \
    __sync_sub_and_fetch(ptr, val)

#define ATOMIC_CAS(ptr, old_val, new_val) \
    __sync_bool_compare_and_swap(ptr, old_val, new_val)

u64 micros();
u64 millis();