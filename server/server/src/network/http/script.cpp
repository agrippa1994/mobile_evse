#include <evse/network/tcp/http/script.hpp>
#include <evse/network/tcp/http/reply.hpp>
#include <evse/network/tcp/http/script_manager.hpp>
#include <evse/network/tcp/http/client.hpp>
#include <evse/network/tcp/http/server.hpp>

bool evse::network::tcp::http::script::exec()
{
    server *svr = (server *)m_reply->getClient()->getServer();
    script_manager& mgr = svr->getScriptManager();
    
    for(auto i : mgr.getFunctions())
        boost::replace_all(m_content, i.name, i.func());

    return true;
}

evse::network::tcp::http::script::script(reply *rpl, const string& path) : m_reply(rpl), m_path(path), m_success(false)
{
    size_t offset = m_path.find(".");
    string extension(m_path.begin() + offset + 1, m_path.end());
    
    ifstream stream(m_path.c_str(), ios_base::in | ios_base::binary);
    if(stream.is_open())
    {
        char buf[512] = {0};
        while(stream.read(buf, sizeof(buf)).gcount() > 0)
            m_content.append(buf, stream.gcount());
    }
    
    // Wenn es keine HTML Datei ist, dann soll die Datei nicht geparsed werden!
    if(!boost::iequals(string("html"), extension))
    {
        m_success = true;
        return;
    }
    
    m_success = exec();
}
