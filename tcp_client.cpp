#include <errno.h>
#include <lwip/netdb.h>
#include <esp_log.h>
#include <arpa/inet.h>
#include "tcp_client.hpp"

#include "socket_utils.hpp"
#include "starrynet_config.hpp"

#define TAG "tcp_socket"

using namespace starrynet;

int tcp_client::connect()
{
    struct sockaddr_in6 remote_addr = {};
    struct timeval recv_tv = {};
    struct timeval send_tv = {};

    bzero(&remote_addr, sizeof(struct sockaddr_in6));

    // Try treat the address as domain name first. If fails, try IPv4
    if (resolve_dns(host, &remote_addr) < 0) {
        return -1;
    }

    if (inet_pton(AF_INET6, host, &remote_addr.sin6_addr) != 1) return -1;


    // Create master socket
    ESP_LOGD(TAG, "Creating TCP socket...");
    if((listen_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        ESP_LOGE(TAG, "Failed to create TCP socket!");
        return -1;
    }

    // Set timeout
    socket_utils::millisec_to_timeval(recv_timeout, &recv_tv);
    socket_utils::millisec_to_timeval(send_timeout, &send_tv);
    setsockopt(listen_fd, SOL_SOCKET, SO_RCVTIMEO, &recv_tv, sizeof(recv_tv));
    setsockopt(listen_fd, SOL_SOCKET, SO_SNDTIMEO, &send_tv, sizeof(send_tv));

    ESP_LOGD(TAG, "Connecting to server: %s:%d, Socket FD: %d...",
             ipaddr_ntoa((const ip_addr_t*)&remote_addr.sin6_addr.un.u32_addr), port, listen_fd);

    if (::connect(listen_fd, (struct sockaddr *)(&remote_addr), sizeof(struct sockaddr)) != 0) {
        close(listen_fd);
        listen_fd = -1;
        return -1;
    }

    return 0;
}

int tcp_client::resolve_dns(const char *host, struct sockaddr_in6 *sockaddr)
{
    struct hostent *he = nullptr;
    struct in_addr **addr_list = nullptr;

    he = gethostbyname(host);

    if (he == nullptr) {
        return -1;
    }

    addr_list = (struct in_addr **)he->h_addr_list;

    if (addr_list[0] == nullptr) {
        return -1;
    }

    sockaddr->sin6_family = AF_INET6;

    memcpy(&sockaddr->sin6_addr, addr_list[0], sizeof(sockaddr->sin6_addr));
    return 0;
}

tcp_client_builder &tcp_client_builder::set_send_timeout(time_t val)
{
    socket.send_timeout = val;
    return *this;
}

tcp_client_builder &tcp_client_builder::set_recv_timeout(time_t val)
{
    socket.recv_timeout = val;
    return *this;
}

tcp_client_builder &tcp_client_builder::set_port(uint16_t val)
{
    socket.port = val;
    return *this;
}

tcp_client_builder &tcp_client_builder::set_host(const std::string &val)
{
    socket.host = strdup(val.c_str());
    return *this;
}

tcp_client &tcp_client_builder::build()
{
    return socket;
}