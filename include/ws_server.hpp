#pragma once

#include <string>
#include <array>
#include <cstdint>
#include <cstring>
#include <ctime>

#include <sys/socket.h>

#include "../starrynet_config.hpp"

namespace starrynet
{
    class ws_server_builder;
    class ws_server
    {
        public:
            friend class ws_server_builder;
            int serve();

        private:
            ws_server() = default;
            time_t send_timeout = 30;
            time_t recv_timeout = 30;
            uint16_t port = 0;
            uint16_t backlog_conn = 5;

            TaskHandle_t server_task_handle = nullptr;
            BaseType_t server_task_core_id = tskNO_AFFINITY;
            uint16_t server_task_priority = tskIDLE_PRIORITY + 5;

            int listen_fd = -1;
            struct sockaddr_in6 serv_addr = {};

            std::array<int, STARRYNET_SERVER_MAX_CLIENT> client_sockets;
            std::array<uint8_t, STARRYNET_WS_BUF_SIZE> buffer;

        private:
            int serve_init();
            static void serve_worker(void *ptr);
    };

    class ws_server_builder
    {
        public:
        ws_server_builder& set_send_timeout(time_t val);
        ws_server_builder& set_recv_timeout(time_t val);
        ws_server_builder& set_port(uint16_t val);
        ws_server_builder& set_backlog_conn(uint16_t val);
        ws_server_builder& set_task_priority(uint16_t val);
            ws_server& build();

        private:
            ws_server server;
    };

}
