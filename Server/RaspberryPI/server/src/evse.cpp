#include <evse/evse.hpp>

#include <boost/log/trivial.hpp>
#include <boost/thread.hpp>

#include <memory>

evse::evse_tcp_client::evse_tcp_client(class evse *ptr, boost::asio::ip::tcp::socket sock)
    : tcp_socket<evse_tcp_client>(ptr, std::move(sock))
{
    BOOST_LOG_TRIVIAL(info) << "[TCP] Einkommende Verbindung";
}

bool evse::evse_tcp_client::send(const std::string &str)
{
    try
    {
        return boost::asio::write(m_socket, boost::asio::buffer(str)) > 0;
    }
    catch(boost::system::error_code & ec)
    {
        BOOST_LOG_TRIVIAL(fatal) << ec.message();
        return false;
    }

    return false;
}

void evse::evse_tcp_client::onData(const std::string &data)
{
    ( (evse *)m_parent )->on_tcp_data(shared_from_this(), data);
}

void evse::evse_tcp_client::onDisconnect()
{
    BOOST_LOG_TRIVIAL(info) << "[TCP] Verbindung wurde von einem Clienten geschlossen";
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

evse::evse::evse() : tcp_server("2425")
{
    BOOST_LOG_TRIVIAL(info) << "[USB] Arduino-Status: " << m_arduino.open();
}

void evse::evse::acceptor()
{
    add_client(std::make_shared<evse_tcp_client>(this, std::move(m_socket)));
}

void evse::evse::on_tcp_data(std::shared_ptr<evse_tcp_client> cl, const std::string &text)
{
    static boost::mutex mtx;
    boost::lock_guard<boost::mutex> p(mtx);

    BOOST_LOG_TRIVIAL(info) << "[TCP] Daten empfangen: " << text;

    bool success = false;
    std::string s = m_arduino.sendAndRead(text + "\r\n", success);

    BOOST_LOG_TRIVIAL(info) << "[USB] Daten empfangen: [" << success << "] " << s;

    bool sent = false;
    if(!success)    sent = cl->send("arduino error!\r\n");
    else            sent = cl->send(s + "\r\n");

    if(sent)    BOOST_LOG_TRIVIAL(info) << "[TCP] Daten wurden gesendet";
    else        BOOST_LOG_TRIVIAL(fatal) << "[TCP] Daten wurden nicht an den Clienten gesendet";

}
