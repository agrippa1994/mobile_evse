#ifndef ARDUINO_H
#define ARDUINO_H
#include "usb_base.h"

#include <boost/algorithm/string.hpp>
#include <boost/function.hpp>

extern boost::function<void (const std::string &)> usb_data;

class arduino : public usb_base
{
public:
    arduino(boost::asio::io_service & io) : usb_base(io) { }
    ~arduino() { }

protected:
    virtual void onError(const boost::system::error_code &);
    virtual void onData(const std::string & data, const size_t);
};

#endif // ARDUINO_H
