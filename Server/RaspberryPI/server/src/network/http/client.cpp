#include <evse/network/tcp/http/client.hpp>
#include <evse/network/tcp/http/server.hpp>
#include <evse/network/tcp/http/request.hpp>
#include <evse/network/tcp/http/reply.hpp>

evse::network::tcp::http::client::client(evse::network::tcp::http::server *ptr, boost::asio::ip::tcp::socket socket)
: tcp_socket<client>(ptr, std::move(socket))
{
    boost::asio::socket_base::send_buffer_size option(65535);
    m_socket.set_option(option);


}

void evse::network::tcp::http::client::onData(size_t bytes)
{
    request req(this, std::string(m_buffer.data()));
    reply rep(this, req, ((server *)m_parent)->getWWW());
    
    std::string data = rep.createReply();


    size_t length = data.length();
    size_t sent = 0;

    while(sent < length)
    {
        try
        {
            sent += m_socket.write_some(boost::asio::buffer(
                                            std::string(
                                                data.begin() + sent,
                                                data.end()
                                                )
                                            )
                                        );
        } catch(...)
        {
            return;
        }
    }
}

void evse::network::tcp::http::client::onDisconnect()
{

}
