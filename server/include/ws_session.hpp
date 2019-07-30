#pragma once

#include <map>
#include <string>
#include <array>
#include <cstdint>
#include <cstring>
#include <ctime>

#include <asio.hpp>

#include "../starrynet_config.hpp"
#include "base_session.hpp"


namespace snet::server
{
    using asio::ip::tcp;
    namespace ws_def
    {
        typedef enum {
            CONTINUATION            = 0,
            TEXT                    = 0x1,
            BINARY                  = 0x2,
            CONNECTION_CLOSE        = 0x8,
            PING                    = 0x9,
            PONG                    = 0xA
        } opcode;

        struct __attribute__((packed)) header {
            bool fin:1;
            uint8_t reserved:3;
            opcode type:4;
            bool masked:1;
            uint8_t payload_len:7;
            uint8_t extras[12];
        };

        struct request {
            bool fin;
            opcode type;
            bool masked;
            uint64_t payload_len;
            uint32_t masking_key;
            uint8_t *payload_ptr;
        };
    }

    class ws_session : public std::enable_shared_from_this<ws_session>, public base_session
    {
        public:
            ws_session(tcp::socket _sock,
                    std::function<void(int)> error_cb,
                    std::function<void(ws_def::request&, ws_session&)>);

            void begin_read() override;

        private:

            void read_length_16();
            void read_length_64();
            void read_mask_key();
            void read_payload();

        private:
            tcp::socket sock;
            std::function<void(int)> error_handler_cb;
            std::function<void(ws_def::request&, ws_session&)> request_handler_cb;
            ws_def::request request{};
            uint8_t mask_key[4] = { 0 };
    };

}
