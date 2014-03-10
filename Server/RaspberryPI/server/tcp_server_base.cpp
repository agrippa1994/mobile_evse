#include "tcp_server_base.h"
#include "log.h"

#include <boost/bind.hpp>

tcp_server_base::tcp_server_base(boost::asio::io_service &io, const std::string& port) : m_socket(io), m_acceptor(io)
{
    LOG(info);

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
    LOG(info);

    boost::lock_guard<boost::mutex> l(m_mtx);

    m_clients.erase(b);
}

void tcp_server_base::sendToAll(const std::string &data)
{
    LOG(info);

    boost::lock_guard<boost::mutex> l(m_mtx);

    // Löschen aller Clienten, die nicht verbunden sind.
    for(auto it = m_clients.begin(); it != m_clients.end();)
    {
        if(!(*it)->is_connected())
            it = m_clients.erase(it);
        else
            it ++;
    }

    for(auto p : m_clients)
        p->send(data);
}

boost::mutex& tcp_server_base::mutex()
{
    return m_mtx;
}

void tcp_server_base::startAcceptHandler()
{
    LOG(info);
    m_acceptor.async_accept(m_socket, boost::bind(&tcp_server_base::acceptHandler, this, boost::asio::placeholders::error));
}

void tcp_server_base::acceptHandler(const boost::system::error_code & ec)
{
    LOG(info) << ec.message();

    boost::lock_guard<boost::mutex> l(m_mtx);

    if(ec)
        return;

    auto ptr = incomingConnection(std::move(m_socket));
    ptr->onConnect();

    m_clients.insert(ptr);

    startAcceptHandler();
}
