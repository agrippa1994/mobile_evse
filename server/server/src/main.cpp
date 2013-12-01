#include <evse/gui/mainwindow.hpp>
#include <evse/std_and_boost.hpp>
#include <evse/network/tcp/http/server.hpp>
#include <evse/rpi/rpi.hpp>

#include <boost/asio/io_service.hpp>

#include <QApplication>

boost::asio::io_service m_io;

void start_network()
{
    try
    {
#ifdef RASPBERRY_PI
    evse::network::tcp::http::server srv(string("http"), string("/home/pi/server/www/"));

    srv.getScriptManager().add_function("RPI_temperature();", [&](){
        stringstream ss; ss << evse::rpi::getTemperature();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_core_voltage();", [&](){
        stringstream ss; ss << evse::rpi::getCoreVoltage();
        return ss.str();
    });

#else
    evse::network::tcp::http::server srv(string("http"), string("/home/mani/Arbeitsfläche/evse_www/"));
#endif
    m_io.run();
    }
    catch(exception& e)
    {
        cout << "Server konnte nicht gestartet werden: " << e.what() << endl;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    cout << "Server wird gestartet ..." << endl;

    // Anzeigen des GUI
    evse::gui::mainwindow w;
    w.show();

    // Starten des HTTP-Servers
    boost::thread thr(start_network);

    return a.exec();
}
