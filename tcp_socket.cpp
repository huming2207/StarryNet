#include <lwip/netdb.h>
#include <esp_log.h>
#include <arpa/inet.h>
#include "tcp_socket.hpp"

#include "socket_utils.hpp"

#define TAG "tcp_socket"

using namespace server;

int tcp_socket::connect()
{
    struct sockaddr_in remote_ip = {};
    struct timeval recv_tv = {};
    struct timeval send_tv = {};

    bzero(&remote_ip, sizeof(struct sockaddr_in));

    // Try treat the address as domain name first. If fails, try IPv4
    if (resolve_dns(host, &remote_ip) < 0) {
        return -1;
    }

    if (inet_pton(AF_INET, host, &remote_ip.sin_addr) != 1) return -1;


    // Create master socket
    ESP_LOGD(TAG, "Creating TCP socket...");
    if((master_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        ESP_LOGE(TAG, "Failed to create TCP socket!");
        return -1;
    }

    // Set timeout
    socket_utils::millisec_to_timeval(recv_timeout, &recv_tv);
    socket_utils::millisec_to_timeval(send_timeout, &send_tv);
    setsockopt(master_fd, SOL_SOCKET, SO_RCVTIMEO, &recv_tv, sizeof(recv_tv));
    setsockopt(master_fd, SOL_SOCKET, SO_SNDTIMEO, &send_tv, sizeof(send_tv));

    ESP_LOGD(TAG, "Connecting to server: %s:%d, Socket FD: %d...",
             ipaddr_ntoa((const ip_addr_t*)&remote_ip.sin_addr.s_addr), port, master_fd);

    if (::connect(master_fd, (struct sockaddr *)(&remote_ip), sizeof(struct sockaddr)) != 0) {
        close(master_fd);
        master_fd = -1;
        return -1;
    }

    return 0;
}

int tcp_socket::resolve_dns(const char *host, struct sockaddr_in *sockaddr)
{
    struct hostent *he = nullptr;
    struct in_addr **addr_list = nullptr;

    he = gethostbyname(host);

    if (he == nullptr) {
        return ESP_FAIL;
    }

    addr_list = (struct in_addr **)he->h_addr_list;

    if (addr_list[0] == nullptr) {
        return ESP_FAIL;
    }

    sockaddr->sin_family = he->h_addrtype; // IPv4 or IPv6??

    memcpy(&sockaddr->sin_addr, addr_list[0], sizeof(sockaddr->sin_addr));
    return ESP_OK;
}

tcp_socket_builder &tcp_socket_builder::set_send_timeout(time_t val)
{
    socket.send_timeout = val;
    return *this;
}

tcp_socket_builder &tcp_socket_builder::set_recv_timeout(time_t val)
{
    socket.recv_timeout = val;
    return *this;
}

tcp_socket_builder &tcp_socket_builder::set_port(uint16_t val)
{
    socket.port = val;
    return *this;
}

tcp_socket_builder &tcp_socket_builder::set_host(const std::string &val)
{
    socket.host = strdup(val.c_str());
    return *this;
}

tcp_socket &tcp_socket_builder::build()
{
    return socket;
}
