#include <evse/network/tcp/http/script_manager.hpp>

#include <sstream>

bool evse::network::tcp::http::script_manager::add_function(std::string name, std::function<void (std::stringstream &)> func)
{
    for(auto i : m_info)
        if(i.name == name)
            return false;
    
    script_info info = {name, func};
    m_info.push_back(info);
    
    return true;
}

bool evse::network::tcp::http::script_manager::remove_function(std::string name, std::function<void (std::stringstream&)> func)
{
    int idx = 0;
    for(auto i : m_info)
    {
        if(i.name == name)
        {
            m_info.erase(m_info.begin() + idx);
            return true;
        }
        
        idx ++;
    }
    
    return false;
}