#include <evse/gui/mainwindow.hpp>
#include <evse/std_and_boost.hpp>
#include <evse/network/tcp/http/server.hpp>

#include <QApplication>

boost::asio::io_service m_io;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    try {
            evse::network::tcp::http::http_server srv(string("http"), string("/home/mani/Arbeitsfläche/evse_www/"));
            srv.run();

        }
        catch (exception& e) {
            cout << e.what() << endl;
            return 0;
        }

    return a.exec();
}
