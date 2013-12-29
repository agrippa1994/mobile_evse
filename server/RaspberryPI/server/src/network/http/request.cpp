#include <evse/network/tcp/http/request.hpp>
#include <evse/network/tcp/http/client.hpp>

#include <boost/algorithm/string.hpp>

evse::network::tcp::http::request::request(client *cl, const std::string& http_header) : m_http_header(http_header), m_client(cl)
{
    // GET zerlegen
    {
        std::vector<std::string> tokens;
        boost::algorithm::split(tokens, m_http_header, boost::algorithm::is_any_of(" "));
    
        for(auto it = tokens.begin(); it != tokens.end(); it ++)
        {
            if(boost::iequals(* it, "get"))
            {
                size_t get_offset = 0;
                std::string get = * (it + 1);
            
                if((get_offset = get.find("?")) != std::string::npos)
                {
                    m_requested_file = std::string(get.begin() + 1, get.begin() + get_offset);
                    std::string getters_str(get.begin() + get_offset + 1, get.end());
                
                    std::vector<std::string> getter_tokens;
                    boost::algorithm::split(getter_tokens, getters_str, boost::algorithm::is_any_of("&"));
                
                    for(auto tok : getter_tokens)
                    {
                        size_t offset = tok.find("=");
                        if(offset != std::string::npos)
                        {
                            m_get_values[std::string(tok.begin(), tok.begin() + offset)] = std::string(tok.begin() + offset + 1, tok.end());
                        }
                    }
                }
                else
                {
                    m_requested_file = std::string(get.begin() + 1, get.end());
                }
            }
        }
    
    if(m_requested_file.length() == 0)
        m_requested_file = "index.html";
    }
    
    // MIME ermitteln
    {
        // Standard setzen
        m_mime_type = "text/html";
        
        size_t offset = m_requested_file.find(".");
        if(offset != std::string::npos)
        {
            struct stInfo{ std::string extension, mime; } mimes[] = {
                {"html", "text/html"},
                {"gif", "text/gif"},
                {"htm", "text/html"},
                {"jpg", "text/jpeg"},
                {"png", "text/png"},
                {"css", "text/css"}
            };
            
            std::string extension(m_requested_file.begin() + offset + 1, m_requested_file.end());
            for(auto i : mimes)
                if(i.extension == extension)
                    m_mime_type = i.mime;
        }
    }
    
    
}
