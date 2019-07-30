#pragma once

namespace snet::server
{
    class base_session
    {
        public:
            virtual void begin_read() = 0;
    };
}