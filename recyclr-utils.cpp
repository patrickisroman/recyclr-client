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
