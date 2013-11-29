#include <evse/network/tcp/http/server.hpp>


evse::network::tcp::http::http_server::http_server(string port, string www) : tcp_server<http_client>(port), m_www(www)
{
    
}

void evse::network::tcp::http::http_server::acceptor()
{
    add_client(make_shared<http_client>(this, move(m_socket)));
}

