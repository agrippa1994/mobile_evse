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

// io_service
boost::asio::io_service g_io;

// I/O-Objekte
server svr(g_io);
arduino ar(g_io);

// TCP-Server Callback
void on_tcp_data(const std::string &data)
{
    ar.send(data);  // Senden der Daten an den Arduino
}

// Arduino Callback
void on_usb_data(const std::string &data)
{
    svr.sendToAll(data);    // Daten an den Server schicken
                            // Der Server schickt die Daten an die Clienten
}

int main()
{
    tcp_data = on_tcp_data;
    usb_data = on_usb_data;

    // Starten des io_service
    g_io.run();
    return 0;
}

