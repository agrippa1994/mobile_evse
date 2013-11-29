#ifndef REPLY_HPP
#define REPLY_HPP

#include <evse/std_and_boost.hpp>
#include "request.hpp"

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{
    
                // forwards
                class client;

                class reply
                {
                    // Request
                    request& m_request;

                    // Website-Ordner
                    string m_www;

                    // Client, der die Antwort erhalten wird
                    client* m_client;


                public:
                    explicit reply(client *cl, request& req, const string& www);

                    // Erstellen der Antwort
                    string createReply();

                    // Pointer zum Client
                    client *getClient() { return m_client; }

                    // Erstellen der Antwort für den Clienten und erstellen des HTTP-Headers
                    static string makeReply(unsigned int status_code, size_t length, const string& mime, const string& content);

                    // Wandelt den Status-Code für den HTTP-Header um
                    static string statusCodeForHTTPHeaderToString(unsigned int status_code);
                };
    
            }
        }
    }
}

#endif
