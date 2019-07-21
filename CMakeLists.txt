idf_component_register(
            SRCS
                "starrynet_config.hpp"
                "server/ws_session.cpp"
                "server/http_session.cpp"
                "server/http_parser.cpp"
                "server/include/base_session.hpp"
                "server/include/ws_session.hpp"
                "server/include/http_session.hpp"
                "server/include/http_parser.hpp"
            INCLUDE_DIRS
                "" "include"
            REQUIRES mbedtls lwip esp_http_server asio
        )
