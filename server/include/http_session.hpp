#pragma once

#include <memory>

#include <asio.hpp>

#include "base_session.hpp"


namespace snet::server
{
    using namespace asio::ip;

    class http_session : public std::enable_shared_from_this<http_session>, public base_session
    {
        public:
            http_session(tcp::socket _sock, std::function<void(int)> err_cb);
            void begin_read() override;
        private:
            void read_req_body(http_def::req_header &header);
            tcp::socket sock;
            asio::streambuf header_buffer;
            asio::streambuf content_buffer;
            std::function<void(int)> error_handler_cb;

    };
}