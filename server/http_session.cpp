#include <utility>
#include <string_view>
#include <http_parser.hpp>

#include "http_session.hpp"

using namespace snet::server;
using namespace snet::server::http_def;

http_session::http_session(tcp::socket _sock, std::function<void(esp_err_t)> error_cb) :
        sock(std::move(_sock)), error_handler_cb(std::move(error_cb))
{

}

void http_session::begin_read()
{
    auto self(shared_from_this());

    asio::async_read_until(sock, header_buffer, "\r\n\r\n",
           [this, self](std::error_code err_code, size_t len)
           {
               if(err_code) {
                   error_handler_cb(err_code.value());
                   return;
               }

               std::string header_str { asio::buffers_begin(header_buffer.data()),
                                        asio::buffers_begin(header_buffer.data()) + header_buffer.size() };
               http_parser parser(header_str);
               http_def::req_header req{};
               auto ret = parser.parse_request(req);
               if(ret != ESP_OK) {
                   error_handler_cb(ret);
               }

               read_req_body(req);
           });

}

void http_session::read_req_body(http_def::req_header &header)
{
    auto self(shared_from_this());
    asio::buffer_copy(content_buffer, header.body_part);
    sock.async_read_some(content_buffer,
        [this, self, &header](std::error_code err_code, size_t len) {
            
        });
}
