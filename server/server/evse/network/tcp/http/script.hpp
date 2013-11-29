#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <evse/std_and_boost.hpp>

namespace evse {
    namespace network {
        namespace tcp {
            namespace http{
                
                //forwards
                class reply;


                class script
                {
                    // Pfad zur Datei, wo das Script ausgefuehrt werden soll
                    const string& m_path;

                    // Inhalt der Datei
                    string m_content;

                    // War das Script erfolgreich, wurde die Datei gefunden?
                    bool m_success;

                    // Pointer zur Antwort, der das Objekt erstellt hat
                    reply* m_reply;


                    // Ausführen des Scriptes (Ersetzen der Befehle in der HTML Datei mit unseren Text)
                    bool exec();


                public:
                    explicit script(reply *rpl, const string& path);

                    // Erfolgreich
                    bool was_successfull() const { return m_success; }

                    // Inhalt
                    const string& getContent() const { return m_content; }

                    // Pointer zur Antwort
                    reply *getReply() { return m_reply; }
                };
    
            }
        }
    }
}

#endif
