#pragma once

#include <string>
#include <cstdint>
#include <cstring>
#include <ctime>

#include <sys/socket.h>

namespace server
{
    class tcp_socket_builder;
    class tcp_socket
    {
        public:
            friend class tcp_socket_builder;
            static int resolve_dns(const char *host, struct sockaddr_in* sockaddr);
            int connect();
            int serve();

        private:
            tcp_socket() = default;
            time_t send_timeout = 30;
            time_t recv_timeout = 30;
            char *host = nullptr;
            uint16_t port = 0;

        private:
            int master_fd = -1;

    };

    class tcp_socket_builder
    {
        public:
            tcp_socket_builder& set_send_timeout(time_t val);
            tcp_socket_builder& set_recv_timeout(time_t val);
            tcp_socket_builder& set_port(uint16_t val);
            tcp_socket_builder& set_host(const std::string& val);
            tcp_socket& build();

        private:
            tcp_socket socket;
    };


};