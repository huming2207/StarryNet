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
            ~http_client_esp();
            http_client_esp& on_error(const std::function<int(int)>& cb);
            http_client_esp& on_sent(const std::function<int()>& cb);
            http_client_esp& on_received(const std::function<int(const char *, int)>& cb);
            http_client_esp& on_finish(const std::function<int()>& cb);
            http_client_esp& on_sock_connect(const std::function<int()>& cb);
            http_client_esp& on_sock_close(const std::function<int()>& cb);
            http_client_esp& on_header(const std::function<int(const std::string& key, const std::string& val)>& cb);


            http_client_esp& run_get();
            http_client_esp& run_post();
            http_client_esp& run_put();
            http_client_esp& run_patch();
            http_client_esp& run_delete();
            http_client_esp& set_post_field(const std::vector<std::tuple<std::string, std::string>>& field);
            http_client_esp& set_post_field(const std::map<std::string, std::string>& field);
            http_client_esp& set_post_field(const std::string& field);
            http_client_esp& set_header(const std::string& key, const std::string& val);
            int get_length();
            int get_status_code();


        private:
            esp_http_client_config_t config{};
            esp_http_client_handle_t client = nullptr;

            std::function<int(int)> on_error_cb;
            std::function<int(const char *, int)> on_received_cb;
            std::function<int()> on_socket_connect_cb;
            std::function<int(const std::string& key, const std::string& val)> on_header_cb;
            std::function<int()> on_finish_cb;
            std::function<int()> on_socket_close_cb;
            std::function<int()> on_sent_cb;

            static std::string make_field(const std::map<std::string, std::string>& field);
            static std::string make_field(const std::vector<std::tuple<std::string, std::string>>& field);
            static esp_err_t http_event_handler(esp_http_client_event_t *event);
    };
}
