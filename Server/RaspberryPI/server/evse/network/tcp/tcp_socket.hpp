#ifndef TCP_SOCKET_HPP
#define TCP_SOCKET_HPP

#include <evse/network/tcp/tcp_server.hpp>

#include <boost/bind.hpp>
#include <boost/algorithm/string/erase.hpp>

#include <vector>
#include <memory>


namespace evse {
    namespace network {
        namespace tcp {
                
            /*
                Abstrakte Basisklasse für TCP Clienten
            */
            template<typename socket> class tcp_socket : public std::enable_shared_from_this<socket>
            {

                // asynchroner Handler für Lese- und  Verbindungsabruchcallbacks
                void read_disconnect_handler()
                {
                    boost::asio::async_read_until(m_socket, m_buffer, "\r\n", boost::bind(&tcp_socket<socket>::onMessageOrDisconnect, this, _1, _2));

                }

                void onMessageOrDisconnect(const boost::system::error_code& e, std::size_t)
                {
                    if(e)
                    {
                        onDisconnect();
                        m_parent->stop_client(getPtr());
                        return;
                    }


                    std::stringstream ss;
                    std::istream(&m_buffer) >> ss.rdbuf();
                    std::string szData = ss.str();

                    boost::algorithm::erase_all(szData, "\r\n");

                    if(szData.length())
                    {
                        onData(szData);
                    }

                    read_disconnect_handler();
                }

            public:
                explicit tcp_socket(tcp_server<socket> *parent, boost::asio::ip::tcp::socket sock) :
                    m_parent(parent), m_socket(std::move(sock))
                {
                    read_disconnect_handler();
                }

                // Pointer zum Server
                tcp_server<socket> *getServer() { return m_parent; }

                std::shared_ptr<socket> getPtr() { return tcp_socket<socket>::shared_from_this(); }
            protected:
                // Socket
                boost::asio::ip::tcp::socket    m_socket;

                // Mit dem Clienten verknüpfter Server
                tcp_server<socket>* m_parent;

                // Buffer für einkommende Daten
                boost::asio::streambuf m_buffer;


                virtual void onData(const std::string & data) = 0;
                virtual void onDisconnect() = 0;
            };

        }
    }
}

#endif
