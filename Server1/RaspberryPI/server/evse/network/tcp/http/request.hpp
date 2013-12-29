#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{
    
                //forwards
                class client;
                // typedefs
                typedef std::map<std::string, std::string> string_map;

                class request
                {
                    // Kopie des empfangenen Headers
                    std::string m_http_header;

                    // Datei die angefordert worden ist
                    std::string m_requested_file;

                    // map, welche die Paramter für die GET-Anfrage beinhaltet
                    string_map m_get_values;

                    // Content-Type aus dem HTTP-Header (Multipurpose Internet Mail Extension)
                    std::string m_mime_type;

                    // Pointer zum Client, der den Request erstellt hat
                    client* m_client;


                public:
                    explicit request(client *cl, const std::string& http_header);

                    // HTTP-Header
                    const std::string& getHTTPHeader() const { return m_http_header; }

                    // Datei
                    const std::string& getRequestedFile() const { return m_requested_file; }

                    // GET-Anfrage
                    const string_map& getGETValues() const { return m_get_values; }

                    // Content-Type
                    const std::string& getMIMEType() const { return m_mime_type; }

                    // Client Pointer
                    client *getClient() { return m_client; }
                };
    
            }
        }
    }
}

#endif
