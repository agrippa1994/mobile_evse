#include <evse/network/usb/usb.hpp>
#include <evse/network/io_service.hpp>

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/algorithm/string/erase.hpp>

evse::network::usb::usb::usb() : m_serial(evse::network::io_service)
{

}

bool evse::network::usb::usb::open()
{
    if(isOpen())
        return true;

    try
    {
        std::string dev = boost::filesystem::exists("/dev/ttyUSB0") ? ("/dev/ttyUSB0") : ("/dev/ttyUSB1");
        m_serial.open(dev);
    }
    catch(boost::system::error_code & ec)
    {
        BOOST_LOG_TRIVIAL(fatal) << "[USB] Fehler beim Verbinden: " << ec.message();
        return false;
    }

    m_serial.set_option(boost::asio::serial_port_base::baud_rate(115200));

    return isOpen();
}

bool evse::network::usb::usb::isOpen() const
{
    return m_serial.is_open();
}

std::string evse::network::usb::usb::sendAndRead(const std::string & data, bool & success)
{
    std::string retn;
    if(!isOpen())
    {
        success = false;
        return retn;
    }

    // Senden der Daten
    {
        try
        {
            std::size_t written = boost::asio::write(m_serial, boost::asio::buffer(data));
            if(written <= 0)
            {
                BOOST_LOG_TRIVIAL(error) << "[USB] Daten konnten nicht geschrieben werden";
                success = false;
                return retn;
            }
        }
        catch(...)
        {
            BOOST_LOG_TRIVIAL(error) << "[USB] Daten konnten nicht geschrieben werden";
            success = false;
            return retn;
        }
    }

    // Lesen der Daten
    {
        try
        {
            boost::asio::streambuf buf;
            std::size_t readed = boost::asio::read_until(m_serial, buf, "\r\n");
            if(readed <= 0)
            {
                BOOST_LOG_TRIVIAL(error) << "[USB] Daten konnten nicht gelesen werden";
                success = false;
                return retn;
            }

            std::istream buf_stream(&buf);
            std::stringstream ss;

            buf_stream >> ss.rdbuf();
            retn  = ss.str();

            boost::algorithm::erase_all(retn, "\r\n");

            if(retn.length() > 0)
            {
                success = true;
                return retn;
            }
        }
        catch(...)
        {
            BOOST_LOG_TRIVIAL(error) << "[USB] Daten konnten nicht gelesen werden";
            success = false;
            return retn;
        }

    }

    BOOST_LOG_TRIVIAL(error) << "[USB] Daten konnten nicht verarbeitet werden";

    success = false;
    return retn;
}
