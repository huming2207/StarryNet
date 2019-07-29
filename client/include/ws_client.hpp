#pragma once

#include <functional>

namespace snet::client
{
    class ws_client
    {
        protected:
            std::function<void()> on_connect_cb;
            std::function<void()> on_disconnect_cb;
            std::function<void(int)> on_error_cb;
            std::function<void(const char *, int)> on_receive_cb;

        public:
            virtual ws_client& set_auth(const std::string& user_name, const std::string& password) = 0;
            virtual ws_client& set_cert_pem(const std::string& pem) = 0;
            virtual ws_client& connect() = 0;
            virtual ws_client& on_connect(const std::function<void()>& connect_cb) = 0;
            virtual ws_client& on_disconnect(const std::function<void()>& disconnect_cb) = 0;
            virtual ws_client& on_receive(const std::function<void(const char *, int)>& cb) = 0;
            virtual ws_client& on_error(const std::function<void(int)>& cb) = 0;
            virtual bool is_connected() = 0;
            virtual ws_client& send(const uint8_t *buf, size_t len, uint32_t timeout) = 0;
            virtual ws_client& send(const std::vector<uint8_t>& buf, uint32_t timeout) = 0;
            virtual int close() = 0;
    };
}