#include <utility>

#include <cerrno>
#include <esp_log.h>

#include "ws_session.hpp"
#include "starrynet_config.hpp"

#define TAG "tcp_server"

using namespace snet::server;

ws_session::ws_session(tcp::socket _sock,
        std::function<void(esp_err_t)> error_cb,
        std::function<void(ws_def::request&, ws_session&)> request_cb) :
            sock(std::move(_sock)), error_handler_cb(std::move(error_cb)), request_handler_cb(std::move(request_cb))
{

}

void ws_session::handle_read()
{
    auto self(shared_from_this());
    ws_def::header header{};

    sock.async_read_some(asio::buffer(&header, sizeof(ws_def::header)),
            [this, self, header](std::error_code err_code, size_t len)
            {
                if(err_code) {
                    error_handler_cb(err_code.value());
                    return;
                }

                // RFC6455 Payload length:
                // If the length byte is 0-125, then the actual length is actually 0 to 125;
                // If the length byte is 126, then the actual length is the additional 16-bit integer next to it;
                // If the length byte is 127, then the actual length is the additional 64-bit integer next to it.
                if(header.payload_len == 126) {
                    read_length_16();
                } else if(header.payload_len == 127) {
                    read_length_64();
                } else {
                    request.payload_len = header.payload_len;
                }

                // If header indicates the payload is masked, then read the mask key.
                if(header.masked) {
                    read_mask_key();
                }

                // Read payload buffer
                read_payload();

                // Finally, call request handler
                request_handler_cb(request, *this);
            });
}

void ws_session::read_length_16()
{
    auto self(shared_from_this());

    uint16_t length = 0;
    std::error_code err_code;
    sock.read_some(asio::buffer(&length, sizeof(uint16_t)), err_code);
    if(err_code) error_handler_cb(err_code.value());
}

void ws_session::read_length_64()
{
    auto self(shared_from_this());

    uint64_t length = 0;
    std::error_code err_code;
    sock.read_some(asio::buffer(&length, sizeof(uint64_t)), err_code);
    if(err_code) error_handler_cb(err_code.value());
}

void ws_session::read_mask_key()
{
    auto self(shared_from_this());

    std::error_code err_code;
    sock.read_some(asio::buffer(&mask_key, sizeof(mask_key)), err_code);
    if(err_code) error_handler_cb(err_code.value());
}

void ws_session::read_payload()
{
    auto self(shared_from_this());

    std::error_code err_code;
    request.payload_ptr = new uint8_t[request.payload_len];
    sock.read_some(asio::buffer(&request.payload_ptr, (std::size_t)request.payload_len), err_code);
    if(err_code) error_handler_cb(err_code.value());

    // If masked, de-mask it
    // Ref: https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API/Writing_WebSocket_servers#Reading_and_Unmasking_the_Data
    if(request.masked) {
        for(size_t curr_idx = 0; curr_idx < request.payload_len; curr_idx++) {
            request.payload_ptr[curr_idx] = request.payload_ptr[curr_idx] ^ mask_key[curr_idx % 4];
        }
    }
}
