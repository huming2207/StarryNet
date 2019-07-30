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

    std::vector<char> header_buf;
    asio::async_read_until(sock, header_buf, "\r\n\r\n",
                           [this, self, &header_buf](std::error_code err_code, size_t len)
           {
               if(err_code) {
                   error_handler_cb(err_code.value());
                   return;
               }

               http_parser parser(header_buf.data(), header_buf.size());
               http_def::result result{};
               auto ret = parser.parse_header(result);
               if((ret != ESP_OK) {
                   error_handler_cb(ret);
               }

           });

}
