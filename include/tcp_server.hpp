#pragma once

#include <string>
#include <cstdint>
#include <cstring>
#include <ctime>

#include <sys/socket.h>

namespace starrynet
{
    class tcp_server_builder;
    class tcp_server
    {
        public:
            friend class tcp_server_builder;
            int serve();


        private:
            tcp_server() = default;
            time_t send_timeout = 30;
            time_t recv_timeout = 30;
            uint16_t port = 0;
            uint16_t backlog_conn = 5;

            TaskHandle_t server_task_handle = nullptr;
            BaseType_t server_task_core_id = tskNO_AFFINITY;
            uint16_t server_task_priority = tskIDLE_PRIORITY + 5;

            int listen_fd = -1;


        private:
            int serve_init();
            int serve_worker(void *ptr);
    };

    class tcp_server_builder
    {
        public:
            tcp_server_builder& set_send_timeout(time_t val);
            tcp_server_builder& set_recv_timeout(time_t val);
            tcp_server_builder& set_port(uint16_t val);
            tcp_server_builder& set_backlog_conn(uint16_t val);
            tcp_server_builder& set_task_priority(uint16_t val);
            tcp_server& build();

        private:
            tcp_server server;
    };
}
