#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

#include <string>

#include "arduino.h"
#include "network.h"
#include "log.h"

using namespace std;

boost::asio::io_service g_io;

server svr(g_io);
arduino ar(g_io);

void on_tcp_data(const std::string &data)
{
    LOG(info);
    ar.send(data);
}

void on_usb_data(const std::string &data)
{
    LOG(info);
    svr.sendToAll(data);
}

int main()
{
    LOG(info);

    tcp_data = on_tcp_data;
    usb_data = on_usb_data;

    g_io.run();
    return 0;
}

