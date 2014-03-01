#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

#include <string>

#include "arduino.h"
#include "network.h"

using namespace std;

boost::asio::io_service g_io;


std::string device("/dev/ttyUSB0");

server svr(g_io);
arduino ar(g_io, device);

void on_tcp_data(const std::string &data)
{
    ar.send(data);
}

void on_usb_data(const std::string &data)
{
    svr.sendToAll(data);
}

int main()
{
    tcp_data = on_tcp_data;
    usb_data = on_usb_data;

    g_io.run();
    return 0;
}

