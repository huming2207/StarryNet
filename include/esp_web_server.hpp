#pragma once

#include <string>
#include <map>
#include <functional>
#include <vector>

#include <esp_http_server.h>

#include "web_server.hpp"

struct endpoint_key
{
    const std::string& uri;
    const server_def::http_method method;

    bool operator==(const endpoint_key& rhs)
    {
        return uri == rhs.uri && method == rhs.method;
    };
};

class esp_web_server
{
public:
    esp_web_server();
    ~esp_web_server();
    int32_t add_endpoint(const std::string &uri, const std::function<int32_t(void*)>& callback,
                        server_def::http_method method);
    int32_t remove_endpoint(const std::string &uri);
    int32_t send_payload(void *req, const std::string &payload);
    int32_t send_payload(void *req, const std::vector<uint8_t>& payload);
    int32_t send_chunk(void *req, const std::string &payload);
    int32_t send_chunk(void *req, const std::vector<uint8_t>& payload);
    int32_t end_chunk(void *req);
    int32_t recv_post(void *req, std::vector<uint8_t>& out);


private:
    httpd_handle_t httpd_handle{};
    std::map<endpoint_key, std::function<int32_t(void*)>> endpoint_map{};
    static server_def::http_method convert_method(int method);
    static int convert_method(server_def::http_method method);
};

