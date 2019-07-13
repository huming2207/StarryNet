#pragma once

#include <string>
#include <array>
#include <cstdint>
#include <cstring>
#include <ctime>

#include <asio.hpp>

#include <sys/socket.h>

#include "../starrynet_config.hpp"

using asio::ip::tcp;

namespace starrynet
{
    class ws_session : public std::enable_shared_from_this<ws_session>
    {
        public:
            ws_session(tcp::socket _sock);
            void handle_read();

        private:
            tcp::socket sock;
    };

}
