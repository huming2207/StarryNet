#pragma once

#include <cstdint>
#include <cstddef>
#include <esp_err.h>

namespace snet
{
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

        struct __attribute__((packed)) header_raw {
            bool fin:1;
            uint8_t reserved:3;
            opcode type:4;
            bool masked:1;
            uint8_t payload_len:7;
            uint8_t extras[12];
        };

        struct result {
            bool fin;
            opcode type;
            bool masked;
            uint64_t payload_len;
            uint32_t masking_key;
            uint8_t *payload_ptr;
        };
    }

    class ws_parser
    {
        public:
            ws_parser(uint8_t *buf, size_t len);
            esp_err_t parse_header(ws_def::result& result);

        private:
            uint8_t *buffer_ptr;
            size_t buffer_len;
    };
}