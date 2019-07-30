#include <string_view>
#include <esp_err.h>
#include "http_parser.hpp"

using namespace snet::server;
using namespace snet::server::http_def;

http_parser::http_parser(const std::string_view &_http_trasct)
{
    http_trasct = _http_trasct;
}

http_parser::http_parser(const char *str, size_t len)
{
    http_trasct = std::string_view(str, len);
}

std::vector<std::string_view> http_parser::split_str(const std::string_view& text, const std::string& delims)
{
    // Ref: https://stackoverflow.com/a/7408245
    std::vector<std::string_view> tokens;
    std::size_t start = text.find_first_not_of(delims), end = 0;

    while((end = text.find_first_of(delims, start)) != std::string::npos) {
        tokens.emplace_back(text.substr(start, end - start));
        start = text.find_first_not_of(delims, end);
    }

    if(start != std::string::npos) tokens.emplace_back(text.substr(start));
    return tokens;
}

esp_err_t http_parser::parse_header(http_def::result &result_out)
{
    // Take substring of the header part only - i.e. the string between start to the blank line
    auto blank_line = http_trasct.find("\r\n\r\n", 0);
    auto header_part = http_trasct.substr(0, blank_line);

    // Set body bytes start index
    result_out.body_idx = blank_line + 4;

    // Split string to vector of lines
    auto header_lines = split_str(header_part, "\r\n");
    if(header_lines.empty()) return ESP_ERR_INVALID_SIZE;

    // Read the first line, e.g. "GET /whatever.htm HTTP/1.1"
    const auto first_line = split_str(header_lines[0], " ");
    if(first_line.size() < 2) return ESP_ERR_INVALID_SIZE;

    result_out.method = method_map.at(first_line[0]);
    result_out.endpoint = first_line[1];
    result_out.version = first_line.size() < 3 ? HTTP_1_0 : version_map.at(first_line[2]);
    header_lines.erase(header_lines.begin());

    // The rest of the header will be kept in the map
    std::map<std::string_view, std::string_view> headers;
    for(auto& curr_item : header_lines) {
        auto colon_idx = curr_item.find(": ");
        auto item_key = curr_item.substr(0, colon_idx);
        auto item_val = curr_item.substr(colon_idx + 2, curr_item.find("\r\n", colon_idx + 2));

        headers[item_key] = item_val; // Add into header map
    }

    if(headers.empty()) return ESP_ERR_INVALID_SIZE;
    if(headers["Upgrade"] == "websocket" || headers["upgrade"] == "websocket") result_out.ws_upgrade = true;

    // Try parse content length
    auto content_len = headers.find("Content-Length");
    if(content_len != headers.end()) {
        result_out.content_len = std::strtol(std::string(content_len->second).c_str(), nullptr, 10);
    } else {
        result_out.content_len = -1;
    }

    result_out.headers = std::move(headers);

    return ESP_OK;
}



const std::map<std::string_view, version> http_parser::version_map  = {
        { "HTTP/1.0", HTTP_1_0 },
        { "HTTP/1.1", HTTP_1_1 },
        { "HTTP/2", HTTP_2 }
};

const std::map<std::string_view, method> http_parser::method_map = {
        { "GET", HTTP_GET },
        { "POST", HTTP_POST },
        { "DELETE", HTTP_DELETE },
        { "PATCH", HTTP_PATCH },
        { "PUT", HTTP_PUT }
};