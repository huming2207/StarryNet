#pragma once

#include <map>
#include <string>
#include <array>
#include <cstdint>
#include <cstring>
#include <ctime>

#include <asio.hpp>

#include <ws_parser.hpp>

#include "../starrynet_config.hpp"


namespace snet
{
    using asio::ip::tcp;
    class ws_session : public std::enable_shared_from_this<ws_session>
    {
        public:
            ws_session(tcp::socket _sock,
                    std::function<void(std::error_code)> error_cb,
                    std::function<void(ws_def::request&, ws_session&)>);

        private:
            void read_header();
            void read_length_16();
            void read_length_64();
            void read_mask_key();
            void read_payload();

        private:
            tcp::socket sock;
            std::function<void(std::error_code)> error_handler_cb;
            std::function<void(ws_def::request&, ws_session&)> request_handler_cb;
            ws_def::request request{};
            uint8_t mask_key[4] = { 0 };
    };

}
