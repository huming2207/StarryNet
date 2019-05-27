#pragma once

#include <sys/time.h>
#include <cstdint>

class socket_utils
{
    public:
        static void millisec_to_timeval(uint32_t time_ms, struct timeval *tv);
};

