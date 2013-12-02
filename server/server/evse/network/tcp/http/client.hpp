#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <evse/network/tcp/tcp_socket.hpp>

#include <boost/asio.hpp>

#define HTTP_MTU 32

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{
                
                class client : public tcp_socket<client>
                {
                public:
                    client(class server *ptr, boost::asio::ip::tcp::socket socket);

                protected:
                    // Daten-Callback
                    virtual void onData(size_t bytes);

                    // Callback für Verbindungsabbau
                    virtual void onDisconnect();
                };

            }
        }
    }
}

#endif
