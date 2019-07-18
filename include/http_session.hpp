#pragma once

#include <memory>

#include <asio.hpp>


namespace snet
{
    using namespace asio::ip;

    class http_session : public std::enable_shared_from_this<http_session>
    {
        public:
            http_session(tcp::socket _sock, std::function<void(esp_err_t)> err_cb);
            void read_header();
        private:
            char http_buf[256];
            tcp::socket sock;
            std::function<void(esp_err_t)> error_handler_cb;

    };
}