#include "tcp_client_base.h"
#include "tcp_server_base.h"

#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

tcp_client_base::tcp_client_base(tcp_server_base *base, boost::asio::ip::tcp::socket sock) : m_parent(base), m_socket(std::move(sock))
{
    BOOST_LOG_TRIVIAL(info) << "tcp_client_base::tcp_client_base";
    startReadHandler();
}

bool tcp_client_base::send(const std::string & data)
{
    BOOST_LOG_TRIVIAL(info) << "tcp_client_base::send";

    if(!m_socket.is_open())
        return false;

    try
    {
        return m_socket.send(boost::asio::buffer(data)) > 0;
    }
    catch(...) { }
    return false;
}

bool tcp_client_base::disconnect()
{
    BOOST_LOG_TRIVIAL(info) << "tcp_client_base::disconnect";

    if(!m_socket.is_open())
        return false;

    try
    {
        m_socket.close();
    }
    catch(...) { return false; }
    return true;
}

void tcp_client_base::startReadHandler()
{
    BOOST_LOG_TRIVIAL(info) << "tcp_client_base::startReadHandler";

    auto p = boost::bind(&tcp_client_base::readHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
    boost::asio::async_read_until(m_socket, m_buf, "\r\n", p);
}

boost::asio::ip::tcp::socket & tcp_client_base::socket()
{
    BOOST_LOG_TRIVIAL(info) << "tcp_client_base::socket";
    return m_socket;
}

class tcp_server_base *tcp_client_base::parent()
{
    BOOST_LOG_TRIVIAL(info) << "tcp_client_base::parent";
    return m_parent;
}

void tcp_client_base::readHandler(const boost::system::error_code & ec, const size_t bytes)
{
    BOOST_LOG_TRIVIAL(info) << "tcp_client_base::readHandler: " << ec.message();
    if(ec)
    {
        onError(ec);
        return;
    }

    std::stringstream ss;
    std::istream(&m_buf) >> ss.rdbuf();

    const std::string & ss_str = ss.str();

    std::string content(ss_str.begin(), ss_str.begin() + bytes);

    onData(content, bytes);

    startReadHandler();
}
