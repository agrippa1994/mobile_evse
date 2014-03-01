#ifndef NETWORK_H
#define NETWORK_H

#include "tcp_client_base.h"
#include "tcp_server_base.h"

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

extern boost::function<void (const std::string &)> tcp_data;

class server : public tcp_server_base
{
public:
    server(boost::asio::io_service & io);

protected:
    virtual boost::shared_ptr<tcp_client_base> incomingConnection(boost::asio::ip::tcp::socket s);
};

class client : public tcp_client_base
{
public:
    client(server *svr, boost::asio::ip::tcp::socket sock);

protected:
    virtual void onConnect();
    virtual void onData(const std::string & data, const size_t);
    virtual void onError(const boost::system::error_code &);
};

#endif // NETWORK_H
