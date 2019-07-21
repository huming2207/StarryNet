#pragma once

namespace snet::server
{
    class base_session
    {
        public:
            virtual void handle_read() = 0;
    };
}