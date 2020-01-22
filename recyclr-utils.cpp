#include "recyclr-utils.h"

u64 micros()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (tp.tv_sec * MICROS_PER_SECOND) + tp.tv_usec;
}

u64 millis()
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (tp.tv_sec * MILLIS_PER_SECOND) + (tp.tv_usec / MICROS_PER_MILLI);
}

u32 set_thread_affinity(std::thread* thr, thread_mask affinity)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(affinity, &cpu);
    return pthread_setaffinity_np(thr->native_handle(), sizeof(cpu), &cpu);
}

u32 set_process_affinity(thread_mask affinity)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(affinity, &cpu);
    return sched_setaffinity(0, sizeof(cpu), &cpu);
}