#ifndef SERVER_HPP
#define SERVER_HPP

#include <evse/network/tcp/tcp_server.hpp>
#include <evse/network/tcp/http/client.hpp>
#include <evse/network/tcp/http/script_manager.hpp>

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{

                class server : public tcp_server<client>
                {
                    // Pfad zu den HTML Dateien
                    std::string m_www;

                    // Script-Manager des HTTP-Servers
                    script_manager m_script_manager;


                public:
                    server(std::string port, std::string www);

                    // Gibt den Pfad zur Website zurück
                    const std::string& getWWW() const { return m_www; }

                    // Script-Manager
                    script_manager& getScriptManager() { return m_script_manager; }


                protected:
                    // TCP-Akzeptor
                    virtual void acceptor();
                };
    
            }
        }
    }
}

#endif
