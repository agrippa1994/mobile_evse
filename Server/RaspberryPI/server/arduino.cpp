#include "arduino.h"

boost::function<void (const std::string &)> usb_data;

void arduino::onError(const boost::system::error_code &)
{
}

void arduino::onData(const std::string & data, const size_t)
{
    if(usb_data)
        usb_data(data);
}
