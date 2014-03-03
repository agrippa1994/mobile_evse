#include "network.h"

#include <boost/log/trivial.hpp>

boost::function<void (const std::string &)> tcp_data;

server::server(boost::asio::io_service & io) : tcp_server_base(io, "2425")
{
    BOOST_LOG_TRIVIAL(info) << "server::server";
}

boost::shared_ptr<tcp_client_base> server::incomingConnection(boost::asio::ip::tcp::socket s)
{
    BOOST_LOG_TRIVIAL(info) << "server::incomingConnection";
    return boost::shared_ptr<tcp_client_base>(new client(this, std::move(s)));
}

client::client(server *svr, boost::asio::ip::tcp::socket sock) : tcp_client_base(svr, std::move(sock))
{
    BOOST_LOG_TRIVIAL(info) << "client::client";
}

void client::onConnect()
{
    BOOST_LOG_TRIVIAL(info) << "client::onConnect";
}

void client::onData(const std::string & data, const size_t)
{
    BOOST_LOG_TRIVIAL(info) << "client::onData";

    if(tcp_data)
        tcp_data(data);
}

void client::onError(const boost::system::error_code &ec)
{
    BOOST_LOG_TRIVIAL(info) << "client::onError " << ec.message();
    parent()->remove(shared_from_this());
}
