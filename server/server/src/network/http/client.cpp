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
    
    string data = rep.createReply();

    // Datenpakte in 10.000 Bytes trennen und seriell senden
    vector<string> data_pakets;
    {
        string tmp;
        for(size_t i =0, u=0; i < data.length(); i++, u++)
        {
            tmp += data[i];

            if(i % HTTP_MTU == 0 && i != 0)
            {
                data_pakets.push_back(tmp);
                tmp.clear();
            }
        }
        data_pakets.push_back(tmp);
    }

    for(string& paket : data_pakets){
        m_socket.async_write_some(boost::asio::buffer(paket), [&](const boost::system::error_code& error, size_t bytesSendet){
            if(error){
                cout << "Fehlerhafte Uebertragung: " << error.message() << endl;
            }
        });
    }
}

void evse::network::tcp::http::client::onDisconnect()
{
    cout << "Client hat die Verbindung abgebaut" << endl;
}
