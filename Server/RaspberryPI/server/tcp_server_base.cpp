#include "tcp_server_base.h"

#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

tcp_server_base::tcp_server_base(boost::asio::io_service &io, const std::string& port) : m_socket(io), m_acceptor(io)
{
    BOOST_LOG_TRIVIAL(info) << "tcp_server_base::tcp_server_base";

    boost::asio::ip::tcp::resolver resolver(io);
    boost::asio::ip::tcp::endpoint endpoint = * resolver.resolve({std::string("0.0.0.0"), port});

    m_acceptor.open(endpoint.protocol());
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor.bind(endpoint);
    m_acceptor.listen();

    startAcceptHandler();
}

void tcp_server_base::remove(boost::shared_ptr<tcp_client_base> b)
{
    BOOST_LOG_TRIVIAL(info) << "tcp_server_base::remove";

    boost::lock_guard<boost::mutex> l(m_mtx);

    m_clients.erase(b);
}

void tcp_server_base::sendToAll(const std::string &data)
{
    BOOST_LOG_TRIVIAL(info) << "tcp_server_base::sendToAll";

    boost::lock_guard<boost::mutex> l(m_mtx);

    for(auto p : m_clients)
        p->send(data);
}

void tcp_server_base::startAcceptHandler()
{
    BOOST_LOG_TRIVIAL(info) << "tcp_server_base::startAcceptHandler";
    m_acceptor.async_accept(m_socket, boost::bind(&tcp_server_base::acceptHandler, this, boost::asio::placeholders::error));
}

void tcp_server_base::acceptHandler(const boost::system::error_code & ec)
{
    BOOST_LOG_TRIVIAL(info) << "tcp_server_base::acceptHandler: " << ec.message();

    boost::lock_guard<boost::mutex> l(m_mtx);

    if(ec)
        return;

    auto ptr = incomingConnection(std::move(m_socket));
    ptr->onConnect();

    m_clients.insert(ptr);

    startAcceptHandler();
}
