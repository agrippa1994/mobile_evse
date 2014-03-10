#include "network.h"
#include "log.h"


boost::function<void (const std::string &)> tcp_data;

server::server(boost::asio::io_service & io) : tcp_server_base(io, "2425")
{
    LOG(info);
}

boost::shared_ptr<tcp_client_base> server::incomingConnection(boost::asio::ip::tcp::socket s)
{
    LOG(info);
    return boost::shared_ptr<tcp_client_base>(new client(this, std::move(s)));
}

client::client(server *svr, boost::asio::ip::tcp::socket sock) : tcp_client_base(svr, std::move(sock))
{
    LOG(info);
}

void client::onConnect()
{
    LOG(info);
}

void client::onData(const std::string & data, const size_t)
{
    LOG(info);

    if(tcp_data)
        tcp_data(data);
}
