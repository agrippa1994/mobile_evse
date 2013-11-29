#ifndef HTTP_Server_mani_http_client_h
#define HTTP_Server_mani_http_client_h

#include "../tcp_socket.hpp"

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{
                
            class http_client : public tcp_socket<http_client>
                {
                public:
                    http_client(class http_server *ptr, boost::asio::ip::tcp::socket socket);

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
