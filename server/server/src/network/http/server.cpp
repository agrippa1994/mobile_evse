#include <evse/network/tcp/http/server.hpp>


evse::network::tcp::http::server::server(std::string port, std::string www) : tcp_server<client>(port), m_www(www)
{
    
}

void evse::network::tcp::http::server::acceptor()
{
    add_client(std::make_shared<client>(this, std::move(m_socket)));
}

