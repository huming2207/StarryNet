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
            void set_auth(const std::string& user_name, const std::string& password);
            void set_cert_pem(const std::string& pem);
            int connect();
            void on_connect_change(const std::function<void()>& connect_cb = {},
                                    const std::function<void()>& disconnect_cb = {});
            void on_receive(const std::function<void(const char *, int)>& cb);
            void on_error(const std::function<void(int)>& cb);

        private:
            esp_websocket_client_config_t config{};
            esp_websocket_client_handle_t handle = nullptr;
            std::function<void()> on_connect_cb;
            std::function<void()> on_disconnect_cb;
            std::function<void(int)> on_error_cb;
            std::function<void(const char *, int)> on_receive_cb;
            EventGroupHandle_t ws_event;
    };
}