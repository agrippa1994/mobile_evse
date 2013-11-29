#include <evse/network/tcp/http/client.hpp>
#include <evse/network/tcp/http/server.hpp>
#include <evse/network/tcp/http/request.hpp>
#include <evse/network/tcp/http/reply.hpp>

evse::network::tcp::http::client::client(evse::network::tcp::http::server *ptr, boost::asio::ip::tcp::socket socket)
: tcp_socket<client>(ptr, move(socket))
{
    
}

void evse::network::tcp::http::client::onData(size_t bytes)
{
    request req(this, string(m_buffer.data()));
    reply rep(this, req, ((server *)m_parent)->getWWW());
    
    cout << "Anfrage von " << m_socket.remote_endpoint().address().to_string() << ": " << req.getRequestedFile() << endl;
    for(auto i : req.getGETValues())
        cout << "GET: " << i.first << " : " << i.second << endl;
    
    m_socket.write_some(boost::asio::buffer(rep.createReply()));
}

void evse::network::tcp::http::client::onDisconnect()
{
    cout << "Client hat die Verbindung abgebaut" << endl;
}
