set(COMPONENT_SRCS  "tcp_client.cpp" "socket_utils.cpp" "starrynet_config.hpp" "ws_server.cpp" "http_parser.cpp"
        "include/tcp_client.hpp" "include/socket_utils.hpp" "include/ws_server.hpp" "include/http_parser.hpp")
set(COMPONENT_ADD_INCLUDEDIRS "" "include")

set(COMPONENT_REQUIRES mbedtls lwip esp_http_server)

register_component()
