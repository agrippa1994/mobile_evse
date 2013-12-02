#ifndef SCRIPT_MANAGER_HPP
#define SCRIPT_MANAGER_HPP

#include <vector>
#include <functional>
#include <string>
#include <iosfwd>

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{

                class script_manager
                {
                    //
                    struct script_info
                    {
                        std::string name;
                        std::function<void (std::stringstream&)> func;
                    };

                    // Haltet alle Informationen zum Script
                    std::vector<script_info> m_info;


                public:
                    // Fuegt eine Funktion fuer das Script hinzu
                    bool add_function(std::string name, std::function<void (std::stringstream&)> func);

                    // Loescht eine Funktion
                    bool remove_function(std::string name, std::function<void (std::stringstream &)> func);

                    // Gibt eine Referenz zum Vektor zurueck
                    std::vector<script_info>& getFunctions() { return m_info; }
                };
    
            }
        }
    }
}

#endif
