#include <cerrno>
#include <regex>

#include <esp_log.h>

#include "ws_server.hpp"
#include "starrynet_config.hpp"

#define TAG "tcp_server"

using namespace starrynet;

int ws_server::serve()
{
    if(serve_init() < 1) return -1;

    xTaskCreatePinnedToCore(&ws_server::serve_worker,
                            "starryhttpd",
                            STARRYNET_SERVER_STACK_SIZE,
                            this,
                            server_task_priority,
                            &server_task_handle,
                            server_task_core_id);

}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void ws_server::serve_worker(void *ptr)
{
    // Pass in the class instance
    auto server = reinterpret_cast<ws_server*>(ptr);
    if(!server) return;

    fd_set read_fds;
    int max_fd, new_fd = 0;
    size_t addr_len = sizeof(server->serv_addr);

    server->client_sockets.fill(0);

    while(true) {

        // Clear socket set
        FD_ZERO(&read_fds);

        // Add master socket
        FD_SET(server->listen_fd, &read_fds);
        max_fd = server->listen_fd;

        // Add child sockets to FD set
        for(auto fd : server->client_sockets) {
            if(fd > 0) FD_SET(fd, &read_fds);
            if(fd > max_fd) max_fd = fd;
        }

        // Perform select()
        int ret = select(max_fd + 1 , &read_fds , nullptr , nullptr , nullptr);
        if(ret < 0) {
            ESP_LOGE(TAG, "Failed to select(): %d", errno);
            return;
        }

        // Accept new clients, if exists and possible
        if(FD_ISSET(server->listen_fd, &read_fds)) {
            if((new_fd = accept(server->listen_fd,
                    (struct sockaddr*)&server->serv_addr, (socklen_t*)&addr_len)) < 0) {
                ESP_LOGE(TAG, "Failed to accept(): %d", new_fd);
                return;
            }

            ESP_LOGD(TAG, "Got connection, socket %d", new_fd);
            bool is_client_added = false;
            for(int& client_fd : server->client_sockets) {
                if(client_fd == 0) {
                    client_fd = new_fd;
                    ESP_LOGD(TAG, "Added to client socket array");
                    is_client_added = true;
                    break;
                }
            }

            if(!is_client_added) {
                ESP_LOGE(TAG, "Failed to find a empty slot for the new client!");
                return;
            }
        }

        // Or, perform IO operations
        for(int& client_fd : server->client_sockets) {
            if(FD_ISSET(client_fd, &read_fds)) {
                server->serve_rx(client_fd);
                // TODO: send() after buffer is parsed
            }
        }
    }
}
#pragma clang diagnostic pop

void ws_server::serve_rx(int& client_fd)
{
    buffer.fill(0);
    int ret = read(client_fd, buffer.data(), STARRYNET_WS_BUF_SIZE);
    if(ret < 0) {
        ESP_LOGE(TAG, "Failed to perform read(): %d", errno);
        return;
    } else if(ret == 0) {
        ESP_LOGD(TAG, "Client %d left", client_fd);
        close(client_fd);
        client_fd = 0;
        return;
    }

}

void ws_server::serve_tx(int& client_fd, size_t len)
{

}

int ws_server::serve_init()
{
    if ((listen_fd = socket(PF_INET6, SOCK_STREAM, 0)) < 0) {
        ESP_LOGE(TAG, "Failed to create socket: %d", errno);
        return -1;
    }

    struct in6_addr inaddr_any = IN6ADDR_ANY_INIT;
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

ws_server_builder& ws_server_builder::set_send_timeout(time_t val)
{
    server.send_timeout = val;
    return *this;
}

ws_server_builder& ws_server_builder::set_recv_timeout(time_t val)
{
    server.recv_timeout = val;
    return *this;
}

ws_server_builder& ws_server_builder::set_port(uint16_t val)
{
    server.port = val;
    return *this;
}

ws_server_builder &ws_server_builder::set_task_priority(uint16_t val)
{
    server.server_task_priority = val;
    return *this;
}

ws_server_builder& ws_server_builder::set_backlog_conn(uint16_t val)
{
    server.backlog_conn = val;
    return *this;
}

ws_server& ws_server_builder::build()
{
    return server;
}

