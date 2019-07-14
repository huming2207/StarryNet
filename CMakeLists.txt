idf_component_register(
            SRCS
                "socket_utils.cpp"
                "starrynet_config.hpp"
                "ws_session.cpp"
                "http_parser.cpp"
                "include/socket_utils.hpp"
                "include/ws_session.hpp"
                "include/http_parser.hpp"
            INCLUDE_DIRS
                "" "include"
            REQUIRES mbedtls lwip esp_http_server asio
        )
