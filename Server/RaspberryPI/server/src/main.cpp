#include <evse/network/io_service.hpp>
#include <evse/evse.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>

#include <string>
#include <sstream>
#include <memory>

int main(int argc, char *argv[])
{
    BOOST_LOG_TRIVIAL(info);
    BOOST_LOG_TRIVIAL(info) << "-------------------------------------";
    BOOST_LOG_TRIVIAL(info) << "Der Server wird gestartet!";
    BOOST_LOG_TRIVIAL(info) << "-------------------------------------";

    evse::evse supply_equipment;

    try
    {
        evse::network::io_service.run();
    }
    catch(std::exception& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << "evse::network::io_service meldet: " << e.what();
    }

    while(true) { boost::this_thread::sleep_for(boost::chrono::seconds(1)); }
}
