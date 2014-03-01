#include "usb_base.h"

bool usb_base::send(const std::string & msg)
{
    return (boost::asio::write(m_usb, boost::asio::buffer(msg)) == msg.length()) ? true : false;
}

usb_base::usb_base(boost::asio::io_service & io, const std::string & port)  : m_usb(io)
{
    try
    {
        m_usb.open(port);
        if(m_usb.is_open())
        {
            m_usb.set_option(boost::asio::serial_port::baud_rate(115200));
        }

        startReadHandler();
    }
    catch(const boost::system::error_code & ec)
    {

    }
}


void usb_base::startReadHandler()
{
    auto p = boost::bind(&usb_base::readHandler, this, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error);
    boost::asio::async_read_until(m_usb, m_buf, "\r\n", p);
}


void usb_base::readHandler(const size_t bytes, const boost::system::error_code & ec)
{
    if(ec)
        return onError(ec);

    std::stringstream ss;
    std::istream(&m_buf) >> ss.rdbuf();

    const std::string ss_str = ss.str();
    std::string content(ss_str.begin(), ss_str.begin() + bytes);

    onData(content, bytes);

    startReadHandler();
}
