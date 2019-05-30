set(COMPONENT_SRCS "web_server.cpp" "esp_web_server.cpp" "tcp_client.cpp" "socket_utils.cpp" "starrynet_config.hpp"
                    "tcp_server.cpp"
        "include/esp_web_server.hpp" "include/web_server.hpp" "include/tcp_client.hpp" "include/socket_utils.hpp"
        "include/tcp_server.hpp")
set(COMPONENT_ADD_INCLUDEDIRS "" "include")

set(COMPONENT_REQUIRES mbedtls lwip esp_http_server)

register_component()
