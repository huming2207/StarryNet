#pragma once

#include <cstring>
#include <string>
#include <functional>

#include <esp_websocket_client.h>

#include <ws_client.hpp>

namespace snet::client
{
    class ws_client_esp;

    class ws_client_esp : public ws_client
    {
        public:
            explicit ws_client_esp(const std::string& uri);
            ws_client_esp(const std::string& host, uint16_t port, const std::string& path);
            ws_client_esp& set_auth(const std::string& user_name, const std::string& password) override;
            ws_client_esp& set_cert_pem(const std::string& pem) override;
            ws_client_esp& connect() override;
            ws_client_esp& on_connect(const std::function<void()>& connect_cb) override;
            ws_client_esp& on_disconnect(const std::function<void()>& disconnect_cb) override;
            ws_client_esp& on_receive(const std::function<void(const char *, int)>& cb) override;
            ws_client_esp& on_error(const std::function<void(int)>& cb) override;
            bool is_connected() override;
            ws_client_esp& send(const uint8_t *buf, size_t len, uint32_t timeout) override;
            ws_client_esp& send(const std::vector<uint8_t>& buf, uint32_t timeout) override;
            int close() override;
            ~ws_client_esp();

        private:
            esp_websocket_client_config_t config{};
            esp_websocket_client_handle_t handle = nullptr;
    };
}