#pragma once

#include <array>
#include <vector>
#include <map>
#include <string>

class http_parser
{
    public:
        http_parser(const std::string &_http_trasct);
        http_parser(const char *str, size_t len);
        std::map<std::string, std::string> parse_header(std::vector<std::string>& header_lines);

    private:
        std::vector<std::string> split_str(const std::string& text, const std::string& delims);
        std::string http_trasct;
};

