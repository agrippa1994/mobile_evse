#ifndef HTTP_Server_mani_http_server_h
#define HTTP_Server_mani_http_server_h

#include "../tcp_server.hpp"
#include "client.hpp"
#include "script_manager.hpp"

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{

                class http_server : public tcp_server<http_client>
                {
                    // Pfad zu den HTML Dateien
                    string m_www;

                    // Script-Manager des HTTP-Servers
                    script_manager m_script_manager;


                public:
                    http_server(string port, string www);

                    // Gibt den Pfad zur Website zurück
                    const string& getWWW() const { return m_www; }

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
