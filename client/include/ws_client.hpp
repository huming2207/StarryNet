#pragma once

#include <functional>

namespace snet::client
{
    class ws_client
    {
        protected:
            std::function<void()> on_connect_cb;
            std::function<void()> on_disconnect_cb;
            std::function<void(int)> on_error_cb;
            std::function<void(const char *, int)> on_receive_cb;

        public:
        virtual void set_auth(const std::string& user_name, const std::string& password) = 0;
        virtual void set_cert_pem(const std::string& pem) = 0;
        virtual int connect() = 0;
        virtual void on_connect_change(const std::function<void()>& connect_cb, const std::function<void()>& disconnect_cb) = 0;
        virtual void on_receive(const std::function<void(const char *, int)>& cb) = 0;
        virtual void on_error(const std::function<void(int)>& cb) = 0;
    };
}