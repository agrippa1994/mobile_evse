#ifndef REPLY_HPP
#define REPLY_HPP

#include <string>

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{
    
                // forwards
                class client;
                class request;

                class reply
                {
                    // Request
                    request& m_request;

                    // Website-Ordner
                    std::string m_www;

                    // Client, der die Antwort erhalten wird
                    client* m_client;


                public:
                    explicit reply(client *cl, request& req, const std::string& www);

                    // Erstellen der Antwort
                    std::string createReply();

                    // Pointer zum Client
                    client *getClient() { return m_client; }

                    // Erstellen der Antwort für den Clienten und erstellen des HTTP-Headers
                    static std::string makeReply(unsigned int status_code, size_t length, const std::string& mime, const std::string& content);

                    // Wandelt den Status-Code für den HTTP-Header um
                    static std::string statusCodeForHTTPHeaderToString(unsigned int status_code);
                };
    
            }
        }
    }
}

#endif
