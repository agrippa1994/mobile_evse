#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <evse/network/io_service.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <memory>
#include <string>
#include <set>

namespace evse {
    namespace network {
        namespace tcp {

            template<typename socket_> class tcp_server
            {
                typedef std::shared_ptr<socket_> socket_ptr;


                void accept_handler()
                {
                    m_acceptor.async_accept(m_socket, boost::bind(&tcp_server<socket_>::do_accept, this, _1));
                }

                void do_accept(boost::system::error_code ec)
                {
                    if(ec || !m_acceptor.is_open())
                        return;

                    acceptor();
                    accept_handler();
                }

            public:
                explicit tcp_server(std::string port) : m_acceptor(evse::network::io_service), m_socket(evse::network::io_service)
                {
                        boost::asio::ip::tcp::resolver resolver(evse::network::io_service);
                        boost::asio::ip::tcp::endpoint endpoint = * resolver.resolve({std::string("0.0.0.0"), port});

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

            protected:
                // Sockets
                std::set<socket_ptr> m_clients;

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
