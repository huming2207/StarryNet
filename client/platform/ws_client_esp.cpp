#include "ws_client_esp.hpp"

using namespace snet::client;

ws_client_esp::ws_client_esp(const std::string &uri)
{
    config.uri = uri.c_str();
}

ws_client_esp::ws_client_esp(const std::string &host, uint16_t port, const std::string &path)
{
    config.host = host.c_str();
    config.port = port;
    config.path = path.c_str();
}

ws_client_esp& ws_client_esp::set_auth(const std::string &user_name, const std::string &password)
{
    config.username = user_name.c_str();
    config.password = password.c_str();
    return *this;
}

ws_client_esp& ws_client_esp::set_cert_pem(const std::string &pem)
{
    config.cert_pem = pem.c_str();
    return *this;
}

ws_client_esp& ws_client_esp::on_connect(const std::function<void()>& connect_cb)
{
    on_connect_cb = connect_cb;
    return *this;
}

ws_client_esp& ws_client_esp::on_disconnect(const std::function<void()>& disconnect_cb)
{
    on_disconnect_cb = disconnect_cb;
    return *this;
}

ws_client_esp& ws_client_esp::on_receive(const std::function<void(const char *, int)>& cb)
{
    on_receive_cb = cb;
    return *this;
}

ws_client_esp& ws_client_esp::on_error(const std::function<void(int)>& cb)
{
    on_error_cb = cb;
    return *this;
}

ws_client_esp& ws_client_esp::connect()
{
    handle = esp_websocket_client_init(&config);
    if(handle == nullptr) {
        on_error_cb(ESP_ERR_NO_MEM);
        return *this;
    }

    // Register on_connect event
    esp_websocket_register_events(handle, WEBSOCKET_EVENT_CONNECTED,
          [](void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
              auto client = static_cast<ws_client_esp*>(handler_args);
              client->on_connect_cb();
          }
    , this);

    esp_websocket_register_events(handle, WEBSOCKET_EVENT_DISCONNECTED,
          [](void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
              auto client = static_cast<ws_client_esp*>(handler_args);
              client->on_disconnect_cb();
          }
    , this);


    esp_websocket_register_events(handle, WEBSOCKET_EVENT_DATA,
          [](void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
              auto client = static_cast<ws_client_esp*>(handler_args);
              auto recved = static_cast<esp_websocket_event_data_t*>(event_data);

              client->on_receive_cb(recved->data_ptr, recved->data_len);
          }
    , this);

    esp_websocket_register_events(handle, WEBSOCKET_EVENT_DATA,
          [](void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
              auto client = static_cast<ws_client_esp*>(event_data);
              client->on_error_cb(ESP_FAIL); // No error code available (seems to be)
          }
    , this);

    auto ret = esp_websocket_client_start(handle);
    if(ret != ESP_OK) on_error_cb(ret);

    return *this;
}

bool ws_client_esp::is_connected()
{
    return esp_websocket_client_is_connected(handle);
}

ws_client_esp& ws_client_esp::send(const uint8_t *buf, size_t len, uint32_t timeout)
{
    auto ret = esp_websocket_client_send(handle, (const char *)buf, len, timeout);
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

ws_client_esp& ws_client_esp::send(const std::vector<uint8_t>& buf, uint32_t timeout)
{
    auto ret = esp_websocket_client_send(handle, (const char *)buf.data(), buf.size(), timeout);
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

int ws_client_esp::close()
{
    return esp_websocket_client_stop(handle);
}

ws_client_esp::~ws_client_esp()
{
    if(handle == nullptr) return;
    ESP_ERROR_CHECK(esp_websocket_client_stop(handle));
    ESP_ERROR_CHECK(esp_websocket_client_destroy(handle));
}
