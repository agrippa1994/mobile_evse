#include "arduino.h"
#include "log.h"

boost::function<void (const std::string &)> usb_data;

void arduino::onError(const boost::system::error_code &ec)
{
    LOG(fatal) << ec.message();

    open();
}

void arduino::onData(const std::string & data, const size_t)
{
    LOG(info);

    if(usb_data)
        usb_data(data);
}
