#pragma once

#include <array>
#include <vector>
#include <map>
#include <string>

enum http_method
{
    HTTP_GET = 0,
    HTTP_POST = 1,
    HTTP_PUT = 2,
    HTTP_DELETE = 3,
    HTTP_PATCH = 4
};

enum http_version
{
    HTTP_1_0 = 0,
    HTTP_1_1 = 1,
    HTTP_2 = 2
};



struct http_result
{
    const std::map<std::string, std::string> headers;
    http_method method;
    const std::string endpoint;
    http_version version;
};

class http_parser
{
    public:
        http_parser(const std::string &_http_trasct);
        http_parser(const char *str, size_t len);
        http_result parse_header(std::vector<std::string>& header_lines);

    private:
        std::vector<std::string> split_str(const std::string& text, const std::string& delims);
        std::string http_trasct;
        static const std::map<std::string, http_method> method_map;
        static const std::map<std::string, http_version> version_map;
};

