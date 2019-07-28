#pragma once

#include <map>
#include <vector>
#include <functional>

namespace snet::client
{
    class http_client
    {
        protected:
            std::function<int(int)> on_error_cb;
            std::function<int(const char *, int)> on_received_cb;
            std::function<int()> on_socket_connect_cb;
            std::function<int(const std::string& key, const std::string& val)> on_header_cb;
            std::function<int()> on_finish_cb;
            std::function<int()> on_socket_close_cb;
            std::function<int()> on_sent_cb;

            static std::string make_field(const std::map<std::string, std::string>& field);
            static std::string make_field(const std::vector<std::tuple<std::string, std::string>>& field);

        public:
            virtual http_client& on_error(const std::function<int(int)>& cb) = 0;
            virtual http_client& on_sent(const std::function<int()>& cb) = 0;
            virtual http_client& on_received(const std::function<int(const char *, int)>& cb) = 0;
            virtual http_client& on_finish(const std::function<int()>& cb) = 0;
            virtual http_client& on_sock_connect(const std::function<int()>& cb) = 0;
            virtual http_client& on_sock_close(const std::function<int()>& cb) = 0;
            virtual http_client& on_header(const std::function<int(const std::string& key, const std::string& val)>& cb) = 0;

            virtual http_client& set_post_field(const std::vector<std::tuple<std::string, std::string>>& field) = 0;
            virtual http_client& set_post_field(const std::map<std::string, std::string>& field) = 0;
            virtual http_client& set_post_field(const std::string& field) = 0;
            virtual http_client& set_header(const std::string& key, const std::string& val) = 0;
            virtual int get_length() = 0;
            virtual int get_status_code() = 0;

            virtual http_client& run_get() = 0;
            virtual http_client& run_post() = 0;
            virtual http_client& run_put() = 0;
            virtual http_client& run_patch() = 0;
            virtual http_client& run_delete() = 0;
    };
}