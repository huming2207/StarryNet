set(COMPONENT_SRCS "web_server.cpp" "esp_web_server.cpp" "tcp_socket.cpp" "socket_utils.cpp"
        "include/esp_web_server.hpp" "include/web_server.hpp" "include/tcp_socket.hpp" "include/socket_utils.hpp")
set(COMPONENT_ADD_INCLUDEDIRS "" "include")

set(COMPONENT_REQUIRES mbedtls lwip esp_http_server)

register_component()
