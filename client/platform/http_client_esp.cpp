#include <sstream>
#include "http_client_esp.hpp"

using namespace snet::client;

http_client_esp::http_client_esp(const std::string &url)
{
    config.url = url.c_str();
    config.user_data = this;
    config.event_handler = &http_client_esp::http_event_handler;
    client = esp_http_client_init(&config);
}

http_client_esp::http_client_esp(const std::string &host, const std::string &path)
{
    config.host = host.c_str();
    config.path = path.c_str();
    config.event_handler = &http_client_esp::http_event_handler;
    client = esp_http_client_init(&config);
}

http_client_esp::~http_client_esp()
{
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}

esp_err_t http_client_esp::http_event_handler(esp_http_client_event_t *event)
{
    auto client = static_cast<http_client_esp *>(event->user_data);

    switch(event->event_id) {
        case HTTP_EVENT_ERROR: return client->on_error_cb(-1);
        case HTTP_EVENT_ON_CONNECTED: return client->on_socket_connect_cb();
        case HTTP_EVENT_ON_DATA: return client->on_received_cb((const char *)event->data, event->data_len);
        case HTTP_EVENT_ON_HEADER: return client->on_header_cb(std::string(event->header_key), std::string(event->header_value));
        case HTTP_EVENT_ON_FINISH: return client->on_finish_cb();
        case HTTP_EVENT_DISCONNECTED: return client->on_socket_close_cb();
        case HTTP_EVENT_HEADERS_SENT: return client->on_sent_cb();
        default: return 0; // Mute other useless callbacks (if any)
    }
}

http_client_esp &http_client_esp::run_get()
{
    auto ret = esp_http_client_set_method(client, HTTP_METHOD_GET);
    ret = ret ?: esp_http_client_perform(client);
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

http_client_esp& http_client_esp::run_post()
{
    auto ret = esp_http_client_set_method(client, HTTP_METHOD_POST);
    ret = ret ?: esp_http_client_perform(client);
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

http_client_esp& http_client_esp::run_put()
{
    auto ret = esp_http_client_set_method(client, HTTP_METHOD_PUT);
    ret = ret ?: esp_http_client_perform(client);
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

http_client_esp& http_client_esp::run_patch()
{
    auto ret = esp_http_client_set_method(client, HTTP_METHOD_PATCH);
    ret = ret ?: esp_http_client_perform(client);
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

http_client_esp& http_client_esp::run_delete()
{
    auto ret = esp_http_client_set_method(client, HTTP_METHOD_DELETE);
    ret = ret ?: esp_http_client_perform(client);
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

std::string http_client_esp::make_field(const std::map<std::string, std::string> &field)
{
    // Make it to a post field string
    std::stringstream post_field;
    for(auto& item : field) {
        post_field << item.first << "=" << item.second << "&";
    }

    // Make it a string
    auto field_str = post_field.str();

    // Remove the last '&'
    field_str.pop_back();

    return field_str;
}

std::string http_client_esp::make_field(const std::vector<std::tuple<std::string, std::string>> &field)
{
    // Make it to a post field string
    std::stringstream post_field;
    for(auto& item : field) {
        post_field << std::get<0>(item) << "=" << std::get<1>(item) << "&";
    }

    // Make it a string
    auto field_str = post_field.str();

    // Remove the last '&'
    field_str.pop_back();

    return field_str;
}

http_client_esp &http_client_esp::set_post_field(const std::vector<std::tuple<std::string, std::string>> &field)
{
    auto field_str = make_field(field);
    auto ret = esp_http_client_set_post_field(client, field_str.c_str(), field_str.size());
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

http_client_esp &http_client_esp::set_post_field(const std::map<std::string, std::string> &field)
{
    auto field_str = make_field(field);
    auto ret = esp_http_client_set_post_field(client, field_str.c_str(), field_str.size());
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

http_client_esp &http_client_esp::set_post_field(const std::string &field)
{
    auto ret = esp_http_client_set_post_field(client, field.c_str(), field.size());
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

http_client_esp &http_client_esp::set_header(const std::string& key, const std::string& val)
{
    auto ret = esp_http_client_set_header(client, key.c_str(), val.c_str());
    if(ret != ESP_OK) on_error_cb(ret);
    return *this;
}

int http_client_esp::get_length()
{
    return esp_http_client_get_content_length(client);
}

int http_client_esp::get_status_code()
{
    return esp_http_client_get_status_code(client);
}

http_client_esp& http_client_esp::on_error(const std::function<int(int)>& cb)
{
    on_error_cb = cb;
    return *this;
}

http_client_esp &http_client_esp::on_sent(const std::function<int()> &cb)
{
    on_sent_cb = cb;
    return *this;
}

http_client_esp &http_client_esp::on_received(const std::function<int(const char *, int)> &cb)
{
    on_received_cb = cb;
    return *this;
}

http_client_esp &http_client_esp::on_finish(const std::function<int()> &cb)
{
    on_finish_cb = cb;
    return *this;
}

http_client_esp &http_client_esp::on_sock_connect(const std::function<int()> &cb)
{
    on_socket_connect_cb = cb;
    return *this;
}

http_client_esp &http_client_esp::on_sock_close(const std::function<int()> &cb)
{
    on_socket_close_cb = cb;
    return *this;
}

http_client_esp &http_client_esp::on_header(const std::function<int(const std::string &key, const std::string &val)> &cb)
{
    on_header_cb = cb;
    return *this;
}

