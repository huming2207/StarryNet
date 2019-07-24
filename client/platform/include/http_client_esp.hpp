#pragma once

#include <string>
#include <map>
#include <string_view>
#include <esp_http_client.h>

namespace snet::client
{
    class http_client_esp
    {
        public:
            explicit http_client_esp(const std::string& url);
            http_client_esp() : http_client_esp("") {};

        private:
            esp_http_client_config_t config{};
            static esp_err_t http_event_handler(esp_http_client_event_t *event);
    };
}
