#pragma once

#include <string>
#include <cstdint>
#include <cstring>
#include <ctime>

#include <sys/socket.h>

namespace starrynet
{
    class tcp_client_builder;
    class tcp_client
    {
        public:
            friend class tcp_client_builder;
            static int resolve_dns(const char *host, struct sockaddr_in6* sockaddr);
            int connect();

        private:
            tcp_client() = default;
            time_t send_timeout = 30;
            time_t recv_timeout = 30;
            char *host = nullptr;
            uint16_t port = 0;

            int listen_fd = -1;

    };

    class tcp_client_builder
    {
        public:
            tcp_client_builder& set_send_timeout(time_t val);
            tcp_client_builder& set_recv_timeout(time_t val);
            tcp_client_builder& set_port(uint16_t val);
            tcp_client_builder& set_host(const std::string& val);
            tcp_client& build();

        private:
            tcp_client socket;
    };


};