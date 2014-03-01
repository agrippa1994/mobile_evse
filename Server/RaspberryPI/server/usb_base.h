#ifndef USB_BASE_H
#define USB_BASE_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>

class usb_base
{
public:
    bool send(const std::string & msg);

protected:
    explicit usb_base(boost::asio::io_service & io, const std::string & port);

    usb_base(usb_base &) = delete;
    usb_base(usb_base &&) = delete;

    usb_base& operator = (usb_base &) = delete;

protected:
    virtual void onError(const boost::system::error_code &) = 0;
    virtual void onData(const std::string & data, const size_t bytes) = 0;

private:
    boost::asio::serial_port    m_usb;
    boost::asio::streambuf      m_buf;
    boost::mutex                m_mtx;

    void startReadHandler();
    void readHandler(const size_t bytes, const boost::system::error_code & ec);
};

#endif // USB_BASE_H
