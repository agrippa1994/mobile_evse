#include <evse/network/tcp/http/server.hpp>


evse::network::tcp::http::server::server(string port, string www) : tcp_server<client>(port), m_www(www)
{
    
}

void evse::network::tcp::http::server::acceptor()
{
    add_client(make_shared<client>(this, move(m_socket)));
}

