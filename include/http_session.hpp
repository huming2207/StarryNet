#pragma once

#include <memory>

#include <asio.hpp>


namespace snet
{
    using namespace asio::ip;

    class http_session : public std::enable_shared_from_this<http_session>
    {
        public:

        private:
            tcp::socket sock;
            std::istream& read_until_crlf(std::istream& is, std::string& opt);

    };
}