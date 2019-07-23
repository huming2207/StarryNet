#pragma once

#include <cstring>
#include <string>
#include <functional>

#include <esp_websocket_client.h>

#include <ws_client.hpp>

namespace snet::client
{
    class esp_ws_client;

    class esp_ws_client : public ws_client
    {
        public:
            explicit esp_ws_client(const std::string& uri);
            esp_ws_client(const std::string& host, uint16_t port, const std::string& path);
            esp_ws_client& set_auth(const std::string& user_name, const std::string& password) override;
            esp_ws_client& set_cert_pem(const std::string& pem) override;
            esp_ws_client& connect() override;
            esp_ws_client& on_connect(const std::function<void()>& connect_cb) override;
            esp_ws_client& on_disconnect(const std::function<void()>& disconnect_cb) override;
            esp_ws_client& on_receive(const std::function<void(const char *, int)>& cb) override;
            esp_ws_client& on_error(const std::function<void(int)>& cb) override;
            bool is_connected() override;
            esp_ws_client& send(const uint8_t *buf, size_t len, uint32_t timeout) override;
            esp_ws_client& send(const std::vector<uint8_t>& buf, uint32_t timeout) override;
            int close() override;
            ~esp_ws_client();

        private:
            esp_websocket_client_config_t config{};
            esp_websocket_client_handle_t handle = nullptr;
    };
}