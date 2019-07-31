#pragma once

#include <array>
#include <vector>
#include <map>
#include <string>

namespace snet::server
{
    namespace http_def
    {
        enum method
        {
            HTTP_GET = 0,
            HTTP_POST = 1,
            HTTP_PUT = 2,
            HTTP_DELETE = 3,
            HTTP_PATCH = 4
        };

        enum version
        {
            HTTP_1_0 = 0,
            HTTP_1_1 = 1,
            HTTP_2 = 2
        };

        struct result
        {
            std::map<std::string, std::string> headers;
            method method;
            std::string_view endpoint;
            version version;
            bool ws_upgrade;
            size_t body_idx;
        };
    }


    class http_parser
    {
        public:
            explicit http_parser(const std::string_view &_http_trasct);
            http_parser(const char *str, size_t len);
            esp_err_t parse_header(http_def::result &result_out);

        private:
            static std::vector<std::string> split_str(const std::string & text, const std::string& delims);
            std::string http_trasct;
            static const std::map<std::string, http_def::method> method_map;
            static const std::map<std::string, http_def::version> version_map;
    };

}
