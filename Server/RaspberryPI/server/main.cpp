#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <string>

#include "arduino.h"
#include "network.h"

using namespace std;

boost::asio::io_service g_io;


std::string device((boost::filesystem::exists("/dev/ttyUSB0")) ? "/dev/ttyUSB0" : "/dev/ttyUSB1");

server svr(g_io);
arduino ar(g_io, device);

void on_tcp_data(const std::string &data)
{
    BOOST_LOG_TRIVIAL(info) << "on_tcp_data";
    ar.send(data);
}

void on_usb_data(const std::string &data)
{
    BOOST_LOG_TRIVIAL(info) << "on_usb_data";
    svr.sendToAll(data);
}

int main()
{
    BOOST_LOG_TRIVIAL(info) << "main";

    tcp_data = on_tcp_data;
    usb_data = on_usb_data;

    g_io.run();
    return 0;
}

