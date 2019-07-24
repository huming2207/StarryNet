#include "http_client_esp.hpp"

snet::client::http_client_esp::http_client_esp(const std::string &url)
{
    config.url = url.c_str();
    config.user_data = this;
    config.event_handler = &http_client_esp::http_event_handler;
    esp_http_client_init(&config);
}

esp_err_t snet::client::http_client_esp::http_event_handler(esp_http_client_event_t *event)
{
    auto client = static_cast<http_client_esp *>(event->user_data);

    return 0;
}
