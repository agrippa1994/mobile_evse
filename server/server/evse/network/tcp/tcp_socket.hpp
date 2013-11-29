#ifndef __HTTP_Server_mani__tcp_socket__
#define __HTTP_Server_mani__tcp_socket__

#include <evse/std_and_boost.hpp>
#include "tcp_server.hpp"

namespace evse {
    namespace network {
        namespace tcp {
                
            /*
                Abstrakte Basisklasse für TCP Clienten
            */
            template<typename socket> class tcp_socket : public enable_shared_from_this<socket>
            {

                // asynchroner Handler für Lese- und  Verbindungsabruchcallbacks
                void read_disconnect_handler()
                {
                    m_socket.async_read_some(boost::asio::buffer(m_buffer), bind(&tcp_socket<socket>::onMessageOrDisconnect, this, placeholders::_1, placeholders::_2));
                }

                void onMessageOrDisconnect(const boost::system::error_code& e, std::size_t bytes_transferred)
                {
                    if(e)
                    {
                        onDisconnect();
                        m_parent->stop_client(getPtr());
                        return;
                    }

                    onData(bytes_transferred);

                    read_disconnect_handler();
                }

            public:
                explicit tcp_socket(tcp_server<socket> *parent, boost::asio::ip::tcp::socket sock) :
                    m_parent(parent), m_socket(move(sock)), m_buffer(8192)
                {
                    read_disconnect_handler();
                }

                // Pointer zum Server
                tcp_server<socket> *getServer() { return m_parent; }

                shared_ptr<socket> getPtr() { return tcp_socket<socket>::shared_from_this(); }
            protected:
                // Socket
                boost::asio::ip::tcp::socket    m_socket;

                // Mit dem Clienten verknüpfter Server
                tcp_server<socket>* m_parent;

                // Buffer für einkommende Daten
                vector<char> m_buffer;


                virtual void onData(size_t bytes) = 0;
                virtual void onDisconnect() = 0;
            };

        }
    }
}

#endif
