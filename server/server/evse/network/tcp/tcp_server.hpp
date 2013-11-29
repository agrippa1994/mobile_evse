#ifndef __HTTP_Server_mani__tcp_server__
#define __HTTP_Server_mani__tcp_server__

#include <evse/std_and_boost.hpp>

extern boost::asio::io_service m_io;

namespace evse {
    namespace network {
        namespace tcp {

            template<typename socket_> class tcp_server
            {
                typedef shared_ptr<socket_> socket_ptr;


                void accept_handler()
                {
                    m_acceptor.async_accept(m_socket, bind(&tcp_server<socket_>::do_accept, this, placeholders::_1));
                }

                void do_accept(boost::system::error_code ec)
                {
                    if(ec || !m_acceptor.is_open())
                        return;

                    acceptor();
                    accept_handler();
                }

            public:
                explicit tcp_server(string port) : m_acceptor(m_io), m_socket(m_io)
                {
                        boost::asio::ip::tcp::resolver resolver(m_io);
                        boost::asio::ip::tcp::endpoint endpoint = * resolver.resolve({string("0.0.0.0"), port});

                        m_acceptor.open(endpoint.protocol());
                        m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
                        m_acceptor.bind(endpoint);
                        m_acceptor.listen();

                        accept_handler();
                }

                void add_client(socket_ptr ptr)
                {
                    m_clients.insert(ptr);
                }

                void stop_client(socket_ptr ptr)
                {
                    m_clients.erase(ptr);
                }

                void run()
                {
                    m_io.run();
                }

            protected:
                // Sockets
                set<socket_ptr> m_clients;

                // TCP-Socket für einkommende Verbindungen
                boost::asio::ip::tcp::socket    m_socket;

                // TCP-Akzeptor
                boost::asio::ip::tcp::acceptor  m_acceptor;


                virtual void acceptor() = 0;
            };

        }
    }
}

#endif
