#include <cstring>

#include "esp_web_server.hpp"

static httpd_uri_t endpoint_conf = {};

esp_web_server::esp_web_server()
{
    std::memset(&httpd_handle, 0, sizeof(httpd_handle_t));
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_ERROR_CHECK(httpd_start(&httpd_handle, &config));
}

esp_web_server::~esp_web_server()
{
    ESP_ERROR_CHECK(httpd_stop(httpd_handle));
}

int32_t esp_web_server::add_endpoint(const std::string &uri,
                                     const std::function<int32_t(void *)>& callback, server_def::http_method method)
{
    // Clean up endpoint config struct first
    std::memset(&endpoint_conf, 0, sizeof(httpd_uri_t));

    // Create a Lambda function, and pass this class instance via `req->user_ctx`
    endpoint_conf.user_ctx = this;
    endpoint_conf.handler = [](httpd_req_t *req) {

        // Try cast the class instance from the user_ctx pointer
        auto instance = reinterpret_cast<esp_web_server*>(req->user_ctx);
        if(instance == nullptr) return ESP_ERR_INVALID_STATE;

        // Find the callback from the endpoint callback map
        endpoint_key key = { .uri = req->uri, .method = req-> };
        auto cb = instance->endpoint_map[req->uri];

        // If the callback is found, run it.
        if(cb != nullptr)
            cb(req);
        else
            return ESP_ERR_HTTPD_TASK;

        return ESP_OK;
    };


    endpoint_conf.uri = uri.c_str();
    switch(method) {
        case server_def::HTTP_METHOD_GET: endpoint_conf.method = HTTP_GET; break;
        case server_def::HTTP_METHOD_POST: endpoint_conf.method = HTTP_POST; break;
        case server_def::HTTP_METHOD_PUT: endpoint_conf.method = HTTP_PUT; break;
        case server_def::HTTP_METHOD_DELETE: endpoint_conf.method = HTTP_DELETE; break;
        default: return ESP_ERR_INVALID_STATE; break;
    }

    endpoint_map.emplace(uri, callback);
    return httpd_register_uri_handler(httpd_handle, &endpoint_conf);
}

int32_t esp_web_server::remove_endpoint(const endpoint_key &key, server_def::http_method method)
{
    endpoint_map.erase(uri);

    return ESP_OK;
}

int32_t esp_web_server::send_payload(void *req, const std::string &payload)
{
    return ESP_OK;
}

int32_t esp_web_server::send_payload(void *req, const std::vector<uint8_t> &payload)
{
    return ESP_OK;
}

int32_t esp_web_server::send_chunk(void *req, const std::string &payload)
{
    return ESP_OK;
}

int32_t esp_web_server::send_chunk(void *req, const std::vector<uint8_t> &payload)
{
    return ESP_OK;
}

int32_t esp_web_server::end_chunk(void *req)
{
    return ESP_OK;
}

int32_t esp_web_server::recv_post(void *req, std::vector<uint8_t> &out)
{
    return ESP_OK;
}

server_def::http_method esp_web_server::convert_method(int method)
{
    switch(method) {
        case HTTP_GET: return server_def::HTTP_METHOD_GET;
        case HTTP_POST
    }
}

int esp_web_server::convert_method(server_def::http_method method)
{
    switch(method) {
        case server_def::HTTP_METHOD_GET: return HTTP_GET;
        case server_def::HTTP_METHOD_POST: return HTTP_POST;
        case server_def::HTTP_METHOD_PUT: return HTTP_PUT;
        case server_def::HTTP_METHOD_DELETE: return HTTP_DELETE;
        default: return HTTP_GET;
    }
}

