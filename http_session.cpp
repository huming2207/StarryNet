#include <utility>
#include <http_parser.hpp>

#include "http_session.hpp"


snet::http_session::http_session(tcp::socket _sock, std::function<void(esp_err_t)> error_cb) :
        sock(std::move(_sock)), error_handler_cb(std::move(error_cb))
{

}

void snet::http_session::handle_read()
{
    auto self(shared_from_this());

    sock.async_read_some(asio::buffer(http_buf, 256),
            [this, self](std::error_code err_code, size_t len)
            {
                if(err_code) {
                    error_handler_cb(err_code.value());
                    return;
                }

                // Header is longer than 256 bytes
                if(http_buf[255] != '\0') {
                    error_handler_cb(ESP_ERR_INVALID_SIZE);
                    return;
                }

                // Parse header
                std::string_view hdr_view(http_buf);
                http_parser parser(hdr_view);

                http_def::result result{};
                auto ret = parser.parse_header(result);

                if((ret != ESP_OK) {
                    error_handler_cb(ret);
                }

            });
}
