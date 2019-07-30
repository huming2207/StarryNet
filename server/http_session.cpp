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

    asio::async_read_until(sock, buffer, "\r\n\r\n",
           [this, self](std::error_code err_code, size_t len)
           {
               if(err_code) {
                   error_handler_cb(err_code.value());
                   return;
               }

               http_parser parser(buffer.data(), buffer.size());
               http_def::result result{};
               auto ret = parser.parse_header(result);
               if((ret != ESP_OK) {
                   error_handler_cb(ret);
               }


           });

}

void http_session::read_body()
{

}
