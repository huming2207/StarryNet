#include "http_parser.hpp"

http_parser::http_parser(const std::string &_http_trasct)
{
    http_trasct = _http_trasct;
}

http_parser::http_parser(const char *str, size_t len)
{
    http_trasct = std::string(str, len);
}

std::vector<std::string> http_parser::split_str(const std::string& text, const std::string& delims)
{
    // Ref: https://stackoverflow.com/a/7408245
    std::vector<std::string> tokens;
    std::size_t start = text.find_first_not_of(delims), end = 0;

    while((end = text.find_first_of(delims, start)) != std::string::npos) {
        tokens.emplace_back(text.substr(start, end - start));
        start = text.find_first_not_of(delims, end);
    }

    if(start != std::string::npos) tokens.emplace_back(text.substr(start));
    return tokens;
}

std::map<std::string, std::string> http_parser::parse_header(std::vector<std::string> &header_lines)
{
    std::map<std::string, std::string> headers;

}

