#ifndef TCP_CLIENT_BASE_H
#define TCP_CLIENT_BASE_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class tcp_client_base : public boost::enable_shared_from_this<tcp_client_base>
{
    friend class tcp_server_base;
public:
    tcp_client_base(class tcp_server_base *base, boost::asio::ip::tcp::socket sock);

    bool is_connected();

    bool send(const std::string & data);
    bool disconnect();

protected:
    virtual void onConnect() = 0;
    virtual void onData(const std::string & data, const size_t bytes) = 0;

    void startReadHandler();

    boost::asio::ip::tcp::socket &  socket();

    class tcp_server_base *         parent();

private:
    class tcp_server_base *m_parent;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_buf;

    void readHandler(const boost::system::error_code & ec, const size_t bytes);
    void writeHandler(const boost::system::error_code &ec, const size_t bytes);
};

#endif // TCP_CLIENT_BASE_H
