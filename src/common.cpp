// common.cpp
#include "common.h"
#include <sys/time.h> // for gettimeofday

double get_wall_time()
{
    struct timeval time;
    if (gettimeofday(&time, NULL)) { return 0; }
    return (double)time.tv_sec + (double)time.tv_usec * 0.000001;
}