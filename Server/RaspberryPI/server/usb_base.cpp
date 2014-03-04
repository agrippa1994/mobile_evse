#include "usb_base.h"
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>

bool usb_base::send(const std::string & msg)
{
    BOOST_LOG_TRIVIAL(info) << "usb_base::send";
    return (boost::asio::write(m_usb, boost::asio::buffer(msg)) == msg.length()) ? true : false;
}

usb_base::usb_base(boost::asio::io_service & io)  : m_usb(io)
{
    BOOST_LOG_TRIVIAL(info) << "usb_base::usb_base";

    open();
}

void usb_base::close()
{
    try
    {
        m_usb.close();
    }
    catch(...) { }
}

bool usb_base::open()
{
    BOOST_LOG_TRIVIAL(info) << "usb_base::open";

    close();

    while(!is_open())
    {
        try
        {
            std::string device((boost::filesystem::exists("/dev/ttyUSB0")) ? "/dev/ttyUSB0" : "/dev/ttyUSB1");

            m_usb.open(device);
            if(m_usb.is_open())
            {
                m_usb.set_option(boost::asio::serial_port::baud_rate(115200));
                startReadHandler();
            }
        }
        catch(...)
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(50));
            open();
        }
    }
}

bool usb_base::is_open() const
{
    return m_usb.is_open();
}

void usb_base::startReadHandler()
{
    BOOST_LOG_TRIVIAL(info) << "usb_base::startReadHandler";

    auto p = boost::bind(&usb_base::readHandler, this, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error);
    boost::asio::async_read_until(m_usb, m_buf, "\r\n", p);
}


void usb_base::readHandler(const size_t bytes, const boost::system::error_code & ec)
{
    BOOST_LOG_TRIVIAL(info) << "usb_base::readHandler: " << ec.message();

    if(ec)
        return onError(ec);

    std::stringstream ss;
    std::istream(&m_buf) >> ss.rdbuf();

    const std::string ss_str = ss.str();
    std::string content(ss_str.begin(), ss_str.begin() + bytes);

    onData(content, bytes);

    startReadHandler();
}
