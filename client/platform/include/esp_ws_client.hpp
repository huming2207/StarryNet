#pragma once

#include <cstring>
#include <string>
#include <functional>

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
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
            void set_auth(const std::string& user_name, const std::string& password) override;
            void set_cert_pem(const std::string& pem) override;
            int connect() override;
            void on_connect_change(const std::function<void()>& connect_cb ,
                                    const std::function<void()>& disconnect_cb) override;
            void on_receive(const std::function<void(const char *, int)>& cb) override;
            void on_error(const std::function<void(int)>& cb) override;
            bool is_connected();
            int send(const uint8_t *buf, size_t len, uint32_t timeout);
            int send(const std::vector<uint8_t>& buf, uint32_t timeout);

        private:
            esp_websocket_client_config_t config{};
            esp_websocket_client_handle_t handle = nullptr;
            EventGroupHandle_t ws_event;
    };
}