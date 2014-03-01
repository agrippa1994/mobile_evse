#ifndef TCP_SERVER_BASE_H
#define TCP_SERVER_BASE_H
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <set>

#include "tcp_client_base.h"

class tcp_server_base
{
public:
    tcp_server_base(boost::asio::io_service & io, const std::string &port);

    void remove(boost::shared_ptr<tcp_client_base> b);

    void sendToAll(const std::string & data);
protected:
    virtual boost::shared_ptr<tcp_client_base> incomingConnection(boost::asio::ip::tcp::socket s) = 0;

private:
    boost::asio::ip::tcp::socket                        m_socket;
    boost::asio::ip::tcp::acceptor                      m_acceptor;
    boost::mutex                                        m_mtx;
    std::set< boost::shared_ptr<tcp_client_base> >      m_clients;

    void startAcceptHandler();
    void acceptHandler(const boost::system::error_code & ec);
};

#endif // TCP_SERVER_BASE_H
