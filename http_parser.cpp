#include <string_view>
#include "http_parser.hpp"

http_parser::http_parser(const std::string_view &_http_trasct)
{
    http_trasct = _http_trasct;
}

http_parser::http_parser(const char *str, size_t len)
{
    http_trasct = std::string_view(str, len);
}

std::vector<std::string_view> http_parser::split_str(const std::string& text, const std::string& delims)
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

http_result http_parser::parse_request_hander(std::vector<std::string> &header_lines)
{
    http_result result{};
    std::map<std::string_view, std::string_view> headers;
    if(header_lines.size() < 2) return result;

    const std::vector<std::string_view> first_line = split_str(header_lines[0], " ");
    for(auto& pattern : first_line) {
        auto ver_map_it = version_map.find(pattern);
        if(ver_map_it != version_map.end()) {
            result.version = ver_map_it->second; continue;
        }

        auto method_map_it = method_map.find(pattern);
        if(method_map_it != method_map.end()) {
            result.method = method_map_it->second; continue;
        }
    }


    for(auto& line : header_lines) {
        const auto& curr_header = split_str(line, ": ");
        if(curr_header.size() < 2) break;
        headers[curr_header[0]] = curr_header[1];
    }

    result.headers = std::move(headers);
    return result;
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