#pragma once

#include <string>
#include <map>
#include <string_view>
#include <functional>
#include <esp_http_client.h>

namespace snet::client
{
    class http_client_esp
    {
        public:
            explicit http_client_esp(const std::string& url);
            http_client_esp(const std::string& host, const std::string& path);
            http_client_esp& on_error(const std::function<void(int)>& cb);
            http_client_esp& run_get();
            http_client_esp& run_post(const std::map<std::string, std::string>& field);
            http_client_esp& run_put(const std::map<std::string, std::string>& field);
            http_client_esp& run_patch(const std::map<std::string, std::string>& field);
            http_client_esp& run_post(const std::vector<std::tuple<std::string, std::string>>& field);
            http_client_esp& run_put(const std::vector<std::tuple<std::string, std::string>>& field);
            http_client_esp& run_patch(const std::vector<std::tuple<std::string, std::string>>& field);
            http_client_esp& run_delete();

        private:
            esp_http_client_config_t config{};
            esp_http_client_handle_t client = nullptr;
            std::function<void(int)> on_error_cb;

            std::string make_field(const std::map<std::string, std::string>& field);
            std::string make_field(const std::vector<std::tuple<std::string, std::string>>& field);
            static esp_err_t http_event_handler(esp_http_client_event_t *event);
    };
}
