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

                "client/ws_client.cpp"
                "client/http_client.cpp"
                "client/include/ws_client.hpp"
                "client/include/http_client.hpp"

                "client/platform/ws_client_esp.cpp"
                "client/platform/http_client_esp.cpp"
                "client/platform/include/ws_client_esp.hpp"
                "client/platform/include/http_client_esp.hpp"
            INCLUDE_DIRS
                "" "server/include" "client/include" "client/platform/include"
            REQUIRES mbedtls lwip esp_http_server asio esp_websocket_client esp_http_client
        )
