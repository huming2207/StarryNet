#include "socket_utils.hpp"

void socket_utils::millisec_to_timeval(uint32_t time_ms, struct timeval *tv)
{
    tv->tv_sec = time_ms / 1000;
    tv->tv_usec = (time_ms - (tv->tv_sec * 1000)) * 1000;
}
