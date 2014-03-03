#include "arduino.h"

#include <boost/log/trivial.hpp>

boost::function<void (const std::string &)> usb_data;

void arduino::onError(const boost::system::error_code &ec)
{
    BOOST_LOG_TRIVIAL(fatal) << "arduino::onError: " << ec.message();
}

void arduino::onData(const std::string & data, const size_t)
{
    BOOST_LOG_TRIVIAL(info) << "arduino::onData";

    if(usb_data)
        usb_data(data);
}
