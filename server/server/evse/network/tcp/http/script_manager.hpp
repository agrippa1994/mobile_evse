#ifndef SCRIPT_MANAGER_HPP
#define SCRIPT_MANAGER_HPP

#include <vector>
#include <functional>
#include <string>

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
                        std::function<std::string (void)> func;
                    };

                    // Haltet alle Informationen zum Script
                    std::vector<script_info> m_info;


                public:
                    // Fuegt eine Funktion fuer das Script hinzu
                    bool add_function(std::string name, std::function<std::string (void)> func);

                    // Loescht eine Funktion
                    bool remove_function(std::string name, std::function<std::string (void)> func);

                    // Gibt eine Referenz zum Vektor zurueck
                    std::vector<script_info>& getFunctions() { return m_info; }
                };
    
            }
        }
    }
}

#endif
