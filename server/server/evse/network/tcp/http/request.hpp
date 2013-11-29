#ifndef __HTTP_Server_mani__request__
#define __HTTP_Server_mani__request__

#include <evse/std_and_boost.hpp>

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{
    
                //forwards
                class http_client;
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
                    http_client* m_client;


                public:
                    explicit request(http_client *cl, const string& http_header);

                    // HTTP-Header
                    const string& getHTTPHeader() const { return m_http_header; }

                    // Datei
                    const string& getRequestedFile() const { return m_requested_file; }

                    // GET-Anfrage
                    const string_map& getGETValues() const { return m_get_values; }

                    // Content-Type
                    const string& getMIMEType() const { return m_mime_type; }

                    // Client Pointer
                    http_client *getClient() { return m_client; }
                };
    
            }
        }
    }
}

#endif
