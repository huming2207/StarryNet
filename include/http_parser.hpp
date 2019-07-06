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
    std::map<std::string_view, std::string_view> headers;
    http_method method;
    std::string_view endpoint;
    http_version version;
    bool ws_upgrade;
};

class http_parser
{
    public:
        explicit http_parser(const std::string_view &_http_trasct);
        http_parser(const char *str, size_t len);
        esp_err_t parse_request_hander(http_result &result_out);

    private:
        std::vector<std::string_view> split_str(const std::string_view & text, const std::string& delims);
        std::string_view http_trasct;
        static const std::map<std::string_view, http_method> method_map;
        static const std::map<std::string_view, http_version> version_map;
};

