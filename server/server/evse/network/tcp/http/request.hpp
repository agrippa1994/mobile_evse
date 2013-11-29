#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <evse/std_and_boost.hpp>

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{
    
                //forwards
                class client;
                // typedefs
                typedef map<string, string> string_map;

                class request
                {
                    // Kopie des empfangenen Headers
                    string m_http_header;

                    // Datei die angefordert worden ist
                    string m_requested_file;

                    // map, welche die Paramter für die GET-Anfrage beinhaltet
                    string_map m_get_values;

                    // Content-Type aus dem HTTP-Header (Multipurpose Internet Mail Extension)
                    string m_mime_type;

                    // Pointer zum Client, der den Request erstellt hat
                    client* m_client;


                public:
                    explicit request(client *cl, const string& http_header);

                    // HTTP-Header
                    const string& getHTTPHeader() const { return m_http_header; }

                    // Datei
                    const string& getRequestedFile() const { return m_requested_file; }

                    // GET-Anfrage
                    const string_map& getGETValues() const { return m_get_values; }

                    // Content-Type
                    const string& getMIMEType() const { return m_mime_type; }

                    // Client Pointer
                    client *getClient() { return m_client; }
                };
    
            }
        }
    }
}

#endif
