#include <string_view>
#include <esp_err.h>
#include "http_parser.hpp"

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

esp_err_t http_parser::parse_request_hander(http_result &result_out)
{
    // Take substring of the header part only - i.e. the string between start to the blank line
    auto header_part = http_trasct.substr(0, http_trasct.find("\r\n\r\n", 0));

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
        auto item_val = curr_item.substr(colon_idx + 1);

        // If there's a "[Cc]onnection: [Uu]pgrade", then it must be a WebSocket upgrade request
        result_out.ws_upgrade = (item_key == "Connection" || item_key == "connection")
                                && (item_val == "upgrade" || item_val == "Upgrade");

        headers[item_key] = item_val; // Add into header map
    }

    if(headers.empty()) return ESP_ERR_INVALID_SIZE;
    result_out.headers = std::move(headers);

    return ESP_OK;
}



const std::map<std::string_view, http_version> http_parser::version_map  = {
        { "HTTP/1.0", HTTP_1_0 },
        { "HTTP/1.1", HTTP_1_1 },
        { "HTTP/2", HTTP_2 }
};

const std::map<std::string_view, http_method> http_parser::method_map = {
        { "GET", HTTP_GET },
        { "POST", HTTP_POST },
        { "DELETE", HTTP_DELETE },
        { "PATCH", HTTP_PATCH },
        { "PUT", HTTP_PUT }
};