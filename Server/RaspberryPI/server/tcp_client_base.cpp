#include "tcp_client_base.h"
#include "tcp_server_base.h"
#include "log.h"

#include <boost/bind.hpp>

tcp_client_base::tcp_client_base(tcp_server_base *base, boost::asio::ip::tcp::socket sock) : m_parent(base), m_socket(std::move(sock))
{
    LOG(info);
    startReadHandler();
}

bool tcp_client_base::is_connected()
{
    return m_socket.is_open();
}

bool tcp_client_base::send(const std::string & data)
{
    LOG(info);

    try
    {
        if(!m_socket.is_open())
            return false;

        auto p = boost::bind(&tcp_client_base::writeHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
        m_socket.async_send(boost::asio::buffer(data), p);

    }
    catch(...)
    {
        return false;
    }
    return false;
}

bool tcp_client_base::disconnect()
{
    LOG(info);

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
    LOG(info);

    auto p = boost::bind(&tcp_client_base::readHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
    boost::asio::async_read_until(m_socket, m_buf, "\r\n", p);
}

boost::asio::ip::tcp::socket & tcp_client_base::socket()
{
    LOG(info);
    return m_socket;
}

class tcp_server_base *tcp_client_base::parent()
{
    LOG(info);
    return m_parent;
}

void tcp_client_base::readHandler(const boost::system::error_code & ec, const size_t bytes)
{
    boost::lock_guard<boost::mutex> lock(parent()->mutex());

    LOG(info) << ec.message();

    if(ec)
    {
        try
        {
            m_socket.cancel();
            m_socket.close();
        }
        catch(...){}

        return;
    }

    std::stringstream ss;
    std::istream(&m_buf) >> ss.rdbuf();

    const std::string & ss_str = ss.str();

    std::string content(ss_str.begin(), ss_str.begin() + bytes);

    onData(content, bytes);

    startReadHandler();
}

void tcp_client_base::writeHandler(const boost::system::error_code &ec, const size_t bytes)
{
    boost::lock_guard<boost::mutex> lock(parent()->mutex());

    LOG(info) << ec.message();

    if(!ec)
        return;

    try
    {
        m_socket.cancel();
        m_socket.close();
    }
    catch(...){}
}
