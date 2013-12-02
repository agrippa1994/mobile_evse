#include <evse/gui/mainwindow.hpp>
#include <evse/network/tcp/http/server.hpp>
#include <evse/rpi/rpi.hpp>
#include <evse/network/io_service.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <string>
#include <sstream>
#include <QApplication>

void start_network()
{
    try
    {
#ifdef RASPBERRY_PI
    evse::network::tcp::http::server srv(std::string("http"), std::string("/home/pi/server/www/"));

    srv.getScriptManager().add_function("RPI_temperature();", [&](){
        std::stringstream ss; ss << evse::rpi::getTemperature();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_core_voltage();", [&](){
        std::stringstream ss; ss << evse::rpi::getCorevoltage();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_sdram_c_voltage();", [&](){
        std::stringstream ss; ss << evse::rpi::getSDRAMCvoltage();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_sdram_i_voltage();", [&](){
        std::stringstream ss; ss << evse::rpi::getSDRAMIvoltage();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_sdram_p_voltage();", [&](){
        std::stringstream ss; ss << evse::rpi::getSDRAMPvoltage();
        return ss.str();
    });



    srv.getScriptManager().add_function("RPI_arm_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getARMfrequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_core_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getCorefrequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_h264_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getH264frequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_isp_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getISPfrequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_v3d_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getV3Dfrequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_uart_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getUARTfrequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_pwm_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getPWMfrequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_emmc_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getEMMCfrequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_pixel_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getPixelfrequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_vec_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getVECfrequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_hdmi_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getHDMIfrequency();
        return ss.str();
    });

    srv.getScriptManager().add_function("RPI_dpi_frequency();", [&](){
        std::stringstream ss; ss << evse::rpi::getDPIfrequency();
        return ss.str();
    });


#else
    evse::network::tcp::http::server srv(string("http"), string("/home/mani/Arbeitsfläche/evse_www/"));
#endif
    evse::network::io_service.run();
    }
    catch(std::exception& e)
    {
        std::cout << "Server konnte nicht gestartet werden: " << e.what() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::cout << "Server wird gestartet ..." << std::endl;

    // Anzeigen des GUI
    evse::gui::mainwindow w;
    w.show();

    // Starten des HTTP-Servers
    boost::thread thr(start_network);

    return a.exec();
}
