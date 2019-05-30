#include <esp_log.h>

#include "tcp_server.hpp"
#include "starrynet_config.hpp"


#define TAG "tcp_server"

using namespace starrynet;

int tcp_server::serve()
{
    if(serve_init() < 1) return -1;
    auto serve_task = [](void* ptr) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
        while(true) {

        }
#pragma clang diagnostic pop
    };

    xTaskCreatePinnedToCore(serve_task,
                            "starryhttpd",
                            STARRYNET_HTTPD_SERVER_STACK_SIZE,
                            this,
                            server_task_priority,
                            &server_task_handle,
                            server_task_core_id);

}

int tcp_server::serve_init()
{
    if ((listen_fd = socket(PF_INET6, SOCK_STREAM, 0)) < 0) {
        ESP_LOGE(TAG, "Failed to create socket: %d", errno);
        return -1;
    }

    struct in6_addr inaddr_any = IN6ADDR_ANY_INIT;
    struct sockaddr_in6 serv_addr = {};
    bzero(&serv_addr, sizeof(struct sockaddr_in6));

    serv_addr.sin6_family  = PF_INET6;
    serv_addr.sin6_addr    = inaddr_any;
    serv_addr.sin6_port    = port;

    // Enable address binding reuse
    int enable = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        /* This will fail if CONFIG_LWIP_SO_REUSE is not enabled. But
         * it does not affect the normal working of the HTTP Server */
        ESP_LOGW(TAG, "Failed when enabling SO_REUSEADDR: %d", errno);
    }

    int ret = bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret < 0) {
        ESP_LOGE(TAG, "Failed to bind: %d", errno);
        close(listen_fd);
        return -1;
    }


    if ((ret = listen(listen_fd, backlog_conn)) < 0) {
        ESP_LOGE(TAG, "Failed to listen: %d", errno);
        close(listen_fd);
        return -1;
    }

    return ret;
}

tcp_server_builder& tcp_server_builder::set_send_timeout(time_t val)
{
    server.send_timeout = val;
    return *this;
}

tcp_server_builder& tcp_server_builder::set_recv_timeout(time_t val)
{
    server.recv_timeout = val;
    return *this;
}

tcp_server_builder& tcp_server_builder::set_port(uint16_t val)
{
    server.port = val;
    return *this;
}

tcp_server_builder &tcp_server_builder::set_task_priority(uint16_t val)
{
    server.server_task_priority = val;
    return *this;
}

tcp_server_builder& tcp_server_builder::set_backlog_conn(uint16_t val)
{
    server.backlog_conn = val;
    return *this;
}

tcp_server& tcp_server_builder::build()
{
    return server;
}

