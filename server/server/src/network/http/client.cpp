#include <evse/network/tcp/http/client.hpp>
#include <evse/network/tcp/http/server.hpp>
#include <evse/network/tcp/http/request.hpp>
#include <evse/network/tcp/http/reply.hpp>

evse::network::tcp::http::http_client::http_client(evse::network::tcp::http::http_server *ptr, boost::asio::ip::tcp::socket socket)
: tcp_socket<http_client>(ptr, move(socket))
{
    
}

void evse::network::tcp::http::http_client::onData(size_t bytes)
{
    request req(this, string(m_buffer.data()));
    reply rep(this, req, ((http_server *)m_parent)->getWWW());
    
    cout << "Anfrage von " << m_socket.remote_endpoint().address().to_string() << ": " << req.getRequestedFile() << endl;
    for(auto i : req.getGETValues())
        cout << "GET: " << i.first << " : " << i.second << endl;
    
    m_socket.write_some(boost::asio::buffer(rep.createReply()));
}

void evse::network::tcp::http::http_client::onDisconnect()
{
    cout << "Client hat die Verbindung abgebaut" << endl;
}
