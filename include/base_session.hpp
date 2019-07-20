#pragma once

namespace snet
{
    class base_session
    {
        public:
            virtual void handle_read() = 0;
    };
}