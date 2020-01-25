#include "recyclr-utils.h"
#include <cstring>

u64 micros()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (static_cast<u64>(tp.tv_sec) * MICROS_PER_SECOND) + static_cast<u64>(tp.tv_usec);
}

u64 millis()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (static_cast<u64>(tp.tv_sec) * MILLIS_PER_SECOND) + (static_cast<u64>(tp.tv_usec) / MICROS_PER_MILLI);
}

u32 set_thread_affinity(std::thread* thr, thread_mask affinity)
{
    #ifdef __APPLE__
        #warning CPU Affinity not supported for MacOSx. Build will continue...
        return 0;
    #else
        cpu_set_t cpu;
        CPU_ZERO(&cpu);
        CPU_SET(affinity, &cpu);
        return pthread_setaffinity_np(thr->native_handle(), sizeof(cpu), &cpu);
    #endif
}

u32 set_process_affinity(thread_mask affinity)
{
    #ifdef __APPLE__
        #warning CPU Affinity not supported for MacOSx. Build will continue...
        return 0;
    #else
        cpu_set_t cpu;
        CPU_ZERO(&cpu);
        CPU_SET(affinity, &cpu);
        return sched_setaffinity(0, sizeof(cpu), &cpu);
    #endif
}