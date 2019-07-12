#include <utility>
#include <cstring>

#include "ws_parser.hpp"

using namespace snet;

ws_parser::ws_parser(uint8_t *buf, size_t len) : buffer_ptr(buf), buffer_len(len)
{

}

esp_err_t ws_parser::parse_header(ws_def::result &result)
{
    if(buffer_ptr == nullptr || buffer_len < 2) return ESP_ERR_INVALID_STATE;

    ws_def::header_raw header{};
    std::memcpy(&header, buffer_ptr, sizeof(ws_def::header_raw));
    buffer_ptr += 2;
    buffer_len -= 2; // Skip 2 byte header offset

    result.fin = header.fin;
    result.masked = header.masked;

    uint8_t mask_key[4] = { 0 };

    // RFC6455 Page 29 - Payload length parsing
    // If the length byte is 0-125, then the actual length is actually 0 to 125;
    // If the length byte is 126, then the actual length is the additional 16-bit integer next to it;
    // If the length byte is 127, then the actual length is the additional 64-bit integer next to it.
    if(header.payload_len < 126) {
        result.payload_len = header.payload_len;
        if(header.masked) {
            std::memcpy(mask_key, header.extras, 4);
            buffer_ptr += 4;
            buffer_len -= 4;
        }

    // 16 bit length: 2 byte header + 2 byte length + 4 bytes optional mask key
    } else if(header.payload_len == 126) {
        result.payload_len = ((uint32_t)header.extras[0] << 8U) | header.extras[1];
        buffer_ptr += 2;
        buffer_len -= 2;
        if(header.masked) {
            std::memcpy(mask_key, header.extras + 2, 4);
            buffer_ptr += 4;
            buffer_len -= 4;
        }

    // 64 bit length: 2 byte header + 8 byte length + 4 bytes optional mask key
    } else if(header.payload_len == 127) {
        result.payload_len = ((uint64_t)(header.extras[0]) << 56U) | ((uint64_t)(header.extras[1]) << 48U) |
                             ((uint64_t)(header.extras[2]) << 40U) | ((uint64_t)(header.extras[3]) << 32U) |
                             ((uint64_t)(header.extras[4]) << 24U) | ((uint64_t)(header.extras[5]) << 16U) |
                             ((uint64_t)(header.extras[6]) << 8U) | ((uint64_t)(header.extras[7]));
        buffer_ptr += 8;
        buffer_len -= 8;
        if(header.masked) {
            std::memcpy(mask_key, header.extras + 8, 4);
            buffer_ptr += 4;
            buffer_len -= 4;
        }
    }

    // Decode mask header
    // Ref: https://developer.mozilla.org/en-US/docs/Web/API/WebSockets_API/Writing_WebSocket_servers#Reading_and_Unmasking_the_Data
    if(header.masked) {
        for(size_t curr_idx = 0; curr_idx < buffer_len; curr_idx++) {
            buffer_ptr[curr_idx] = buffer_ptr[curr_idx] ^ mask_key[curr_idx % 4];
        }
    }

    result.payload_ptr = buffer_ptr;
    result.payload_len = buffer_len;

    return ESP_OK;
}