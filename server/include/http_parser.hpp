#pragma once

#include <array>
#include <vector>
#include <map>
#include <string>

namespace snet::server
{
    namespace http_def
    {
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

        struct req_header
        {
            std::map<std::string, std::string> headers;
            http_method method;
            std::string_view endpoint;
            http_version version;
            bool ws_upgrade;
            std::vector<uint8_t> body_part;
            ssize_t body_len;
        };
    }


    class http_parser
    {
        public:
            explicit http_parser(const std::string &_http_trasct);
            http_parser(const char *str, size_t len);
            esp_err_t parse_request(http_def::req_header &result_out);

        private:
            static std::vector<std::string> split_str(const std::string & text, const std::string& delims);
            std::string http_trasct;
            static const std::map<std::string, http_def::http_method> method_map;
            static const std::map<std::string, http_def::http_version> version_map;
    };

}
