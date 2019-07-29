#pragma once

#include <string>
#include <map>
#include <string_view>
#include <functional>
#include <esp_http_client.h>
#include <http_client.hpp>

namespace snet::client
{
    class http_client_esp : public http_client
    {
        public:
            explicit http_client_esp(const std::string& url);
            http_client_esp(const std::string& host, const std::string& path);
            ~http_client_esp();
            http_client_esp& on_error(const std::function<int(int)>& cb) override;
            http_client_esp& on_sent(const std::function<int()>& cb) override;
            http_client_esp& on_received(const std::function<int(const char *, int)>& cb) override;
            http_client_esp& on_finish(const std::function<int()>& cb) override;
            http_client_esp& on_sock_connect(const std::function<int()>& cb) override;
            http_client_esp& on_sock_close(const std::function<int()>& cb) override;
            http_client_esp& on_header(const std::function<int(const std::string& key, const std::string& val)>& cb) override;

            http_client_esp& set_post_field(const std::vector<std::tuple<std::string, std::string>>& field) override;
            http_client_esp& set_post_field(const std::map<std::string, std::string>& field) override;
            http_client_esp& set_post_field(const std::string& field) override;
            http_client_esp& set_header(const std::string& key, const std::string& val) override;
            int get_length() override;
            int get_status_code() override;

            http_client_esp& run_get() override;
            http_client_esp& run_post() override;
            http_client_esp& run_put() override;
            http_client_esp& run_patch() override;
            http_client_esp& run_delete() override;

        private:
            esp_http_client_config_t config{};
            esp_http_client_handle_t client = nullptr;

            static esp_err_t http_event_handler(esp_http_client_event_t *event);
    };
}
