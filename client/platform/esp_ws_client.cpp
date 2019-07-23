#include "esp_ws_client.hpp"

using namespace snet::client;

esp_ws_client::esp_ws_client(const std::string &uri)
{
    config.uri = uri.c_str();
}

esp_ws_client::esp_ws_client(const std::string &host, uint16_t port, const std::string &path)
{
    config.host = host.c_str();
    config.port = port;
    config.path = path.c_str();
}

esp_ws_client& esp_ws_client::set_auth(const std::string &user_name, const std::string &password)
{
    config.username = user_name.c_str();
    config.password = password.c_str();
    return *this;
}

esp_ws_client& esp_ws_client::set_cert_pem(const std::string &pem)
{
    config.cert_pem = pem.c_str();
    return *this;
}

esp_ws_client& esp_ws_client::on_connect(const std::function<void()>& connect_cb)
{
    on_connect_cb = connect_cb;
    return *this;
}

esp_ws_client& esp_ws_client::on_disconnect(const std::function<void()>& disconnect_cb)
{
    on_disconnect_cb = disconnect_cb;
    return *this;
}

esp_ws_client& esp_ws_client::on_receive(const std::function<void(const char *, int)>& cb)
{
    on_receive_cb = cb;
    return *this;
}

esp_ws_client& esp_ws_client::on_error(const std::function<void(int)>& cb)
{
    on_error_cb = cb;
    return *this;
}

esp_ws_client& esp_ws_client::connect()
{
    handle = esp_websocket_client_init(&config);
    if(handle == nullptr) {
        on_error_cb(ESP_ERR_NO_MEM);
        return *this;
    }

    // Register on_connect event
    esp_websocket_register_events(handle, WEBSOCKET_EVENT_CONNECTED,
          [](void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
              auto client = static_cast<esp_ws_client*>(handler_args);
              client->on_connect_cb();
          }
    , this);

    esp_websocket_register_events(handle, WEBSOCKET_EVENT_DISCONNECTED,
          [](void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
              auto client = static_cast<esp_ws_client*>(handler_args);
              client->on_disconnect_cb();
          }
    , this);


    esp_websocket_register_events(handle, WEBSOCKET_EVENT_DATA,
          [](void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
              auto client = static_cast<esp_ws_client*>(handler_args);
              auto recved = static_cast<esp_websocket_event_data_t*>(event_data);

              client->on_receive_cb(recved->data_ptr, recved->data_len);
          }
    , this);

    esp_websocket_register_events(handle, WEBSOCKET_EVENT_DATA,
          [](void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
              auto client = static_cast<esp_ws_client*>(event_data);
              client->on_error_cb(ESP_FAIL); // No error code available (seems to be)
          }
    , this);

    auto ret = esp_websocket_client_start(handle);
    if(ret != ESP_OK) on_error_cb(ret);

    return *this;
}

bool esp_ws_client::is_connected()
{
    return esp_websocket_client_is_connected(handle);
}

esp_ws_client& esp_ws_client::send(const uint8_t *buf, size_t len, uint32_t timeout)
{
    auto ret = esp_websocket_client_send(handle, (const char *)buf, len, timeout);
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

esp_ws_client& esp_ws_client::send(const std::vector<uint8_t>& buf, uint32_t timeout)
{
    auto ret = esp_websocket_client_send(handle, (const char *)buf.data(), buf.size(), timeout);
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}
