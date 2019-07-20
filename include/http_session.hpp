#pragma once

#include <memory>

#include <asio.hpp>

#include "base_session.hpp"


namespace snet
{
    using namespace asio::ip;

    class http_session : public std::enable_shared_from_this<http_session>, public base_session
    {
        public:
            http_session(tcp::socket _sock, std::function<void(int)> err_cb);
            void handle_read() override;
        private:
            char http_buf[256];
            tcp::socket sock;
            std::function<void(int)> error_handler_cb;

    };
}