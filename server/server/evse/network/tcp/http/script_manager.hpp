#ifndef __HTTP_Server_mani__script_manager__
#define __HTTP_Server_mani__script_manager__

#include <evse/std_and_boost.hpp>

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{

                class script_manager
                {
                    //
                    struct script_info
                    {
                        string name;
                        function<string (void)> func;
                    };

                    // Haltet alle Informationen zum Script
                    vector<script_info> m_info;


                public:
                    // Fuegt eine Funktion fuer das Script hinzu
                    bool add_function(string name, function<string (void)> func);

                    // Loescht eine Funktion
                    bool remove_function(string name, function<string (void)> func);

                    // Gibt eine Referenz zum Vektor zurueck
                    vector<script_info>& getFunctions() { return m_info; }
                };
    
            }
        }
    }
}

#endif
