#include "network.h"

boost::function<void (const std::string &)> tcp_data;

server::server(boost::asio::io_service & io) : tcp_server_base(io, "2425")
{

}

boost::shared_ptr<tcp_client_base> server::incomingConnection(boost::asio::ip::tcp::socket s)
{
    return boost::shared_ptr<tcp_client_base>(new client(this, std::move(s)));
}

client::client(server *svr, boost::asio::ip::tcp::socket sock) : tcp_client_base(svr, std::move(sock))
{

}

void client::onConnect()
{
}

void client::onData(const std::string & data, const size_t)
{
    if(tcp_data)
        tcp_data(data);
}

void client::onError(const boost::system::error_code &)
{
    parent()->remove(shared_from_this());
}
