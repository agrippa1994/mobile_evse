#include <evse/gui/mainwindow.hpp>
#include <evse/std_and_boost.hpp>
#include <evse/network/tcp/http/server.hpp>

#include <boost/asio/io_service.hpp>

#include <QApplication>

boost::asio::io_service m_io;

void start_network()
{
    evse::network::tcp::http::server srv(string("http"), string("/home/mani/Arbeitsfläche/evse_www/"));

    m_io.run();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Anzeigen des GUI
    evse::gui::mainwindow w;
    w.show();

    // Starten des HTTP-Servers
    std::thread thr(start_network);
    thr.detach();

    return a.exec();
}
