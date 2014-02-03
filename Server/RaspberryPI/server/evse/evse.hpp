#ifndef EVSE_HPP
#define EVSE_HPP

#include <evse/network/tcp/tcp_server.hpp>
#include <evse/network/tcp/tcp_socket.hpp>
#include <evse/network/usb/usb.hpp>

namespace evse
{
    class evse;
    class evse_tcp_client;

    class evse_tcp_client : public network::tcp::tcp_socket<evse_tcp_client>
    {
    public:
        evse_tcp_client(class evse *ptr, boost::asio::ip::tcp::socket socket);

        bool send(const std::string & str);
    protected:
        virtual void onData(const std::string &data);
        virtual void onDisconnect();
    };

    class evse : public network::tcp::tcp_server<evse_tcp_client>
    {
        friend class evse_tcp_client;
    public:
        evse();

    protected:
        void acceptor();

    private:
        network::usb::usb m_arduino;

        void on_tcp_data(std::shared_ptr<evse_tcp_client> cl, const std::string &text);
    };
}


#endif // EVSE_HPP
