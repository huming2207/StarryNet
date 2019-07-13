#include <cerrno>
#include <esp_log.h>

#include "ws_session.hpp"
#include "starrynet_config.hpp"

#define TAG "tcp_server"

using namespace starrynet;


ws_session::ws_session(tcp::socket _sock) : sock(std::move(_sock))
{

}
