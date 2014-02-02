#ifndef USB_HPP
#define USB_HPP

#include <boost/asio.hpp>

namespace evse
{
    namespace network
    {
        namespace usb
        {
            class usb
            {
            public:
                explicit usb();

                usb(usb&) = delete;
                usb(usb&&) = delete;

                bool open();
                bool isOpen() const;

                std::string sendAndRead(const std::string &data, bool & success);

            private:
                boost::asio::serial_port m_serial;
            };
        }
    }
}


#endif
