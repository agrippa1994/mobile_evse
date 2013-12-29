#include <evse/gui/mainwindow.hpp>
#include <evse/network/tcp/http/server.hpp>
#include <evse/rpi/rpi.hpp>
#include <evse/network/io_service.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>

#include <string>
#include <sstream>
#include <memory>

#include <QApplication>

void start_network()
{
    try
    {
#ifdef RASPBERRY_PI
    evse::network::tcp::http::server srv(std::string("http"), std::string("/home/pi/server/www/"));

    srv.getScriptManager().add_function("RPI_temperature();", [&](std::stringstream& ss){
        ss << evse::rpi::getTemperature();
    });

    srv.getScriptManager().add_function("RPI_core_voltage();", [&](std::stringstream& ss){
        ss << evse::rpi::getCorevoltage();
    });

    srv.getScriptManager().add_function("RPI_sdram_c_voltage();", [&](std::stringstream& ss){
        ss << evse::rpi::getSDRAMCvoltage();
    });

    srv.getScriptManager().add_function("RPI_sdram_i_voltage();", [&](std::stringstream& ss){
        ss << evse::rpi::getSDRAMIvoltage();
    });

    srv.getScriptManager().add_function("RPI_sdram_p_voltage();", [&](std::stringstream& ss){
        ss << evse::rpi::getSDRAMPvoltage();
    });



    srv.getScriptManager().add_function("RPI_arm_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getARMfrequency();
    });

    srv.getScriptManager().add_function("RPI_core_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getCorefrequency();
    });

    srv.getScriptManager().add_function("RPI_h264_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getH264frequency();
    });

    srv.getScriptManager().add_function("RPI_isp_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getISPfrequency();
    });

    srv.getScriptManager().add_function("RPI_v3d_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getV3Dfrequency();
    });

    srv.getScriptManager().add_function("RPI_uart_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getUARTfrequency();
    });

    srv.getScriptManager().add_function("RPI_pwm_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getPWMfrequency();
    });

    srv.getScriptManager().add_function("RPI_emmc_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getEMMCfrequency();
    });

    srv.getScriptManager().add_function("RPI_pixel_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getPixelfrequency();
    });

    srv.getScriptManager().add_function("RPI_vec_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getVECfrequency();
    });

    srv.getScriptManager().add_function("RPI_hdmi_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getHDMIfrequency();
    });

    srv.getScriptManager().add_function("RPI_dpi_frequency();", [&](std::stringstream& ss){
        ss << evse::rpi::getDPIfrequency();
    });


#else
    evse::network::tcp::http::server srv(std::string("http"), std::string("/home/mani/Arbeitsfläche/evse_www/"));
#endif
    evse::network::io_service.run();
    }
    catch(std::exception& e)
    {
        std::cout << "evse::network::io_service meldet: " << e.what() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    std::cout << sizeof(float) << std::endl;
    std::cout << sizeof(int) << std::endl;

    QApplication a(argc, argv);


    std::cout << "Server wird gestartet ..." << std::endl;

    // Anzeigen des GUI
    evse::gui::mainwindow w;
    w.show();

    // Starten des HTTP-Servers
    boost::thread thr(start_network);

    return a.exec();
}