#include "CommandHandler.h"
#include "Vector.h"
#include "String.h"

// Globale Variablen
CommandHandler_Map g_CommandHandler_Map;

// Initialisierung
void CommandHandler_init()
{
  // Hier könnte man "Standard - Befehle" binden
}

// Aufsplitten des Befehles; Übergabe an den Callback Funktionen
void CommandHandler(const String & usb_command)
{
  Vector<String> tokens = split(usb_command, " ");  
  
  // Die Anzahl der Einzelteile darf nicht durch 2 teilbar sein!
  // COMMAND --PARAM1 ARGUMENT1 --PARAM2 ARGUMENT2 --PARAMN ARGUMENTN
  
  if(tokens.size() % 2 == 0 || tokens.size() == 0)
    return;
    
  // Der Befehl steht am Anfang..  
  String command = tokens[0];
  
  // Erstellen der String_Map mit den Argumenten und Parametern
  String_Map args_params;
    for(int i=0; i < (tokens.size() - 1) / 2; i++)
      args_params[ tokens[i*2 + 1] ] = tokens[i*2 + 2];
  
  // Suchen des registrierten Befehles und Aufrufen des Callbacks, falls der Funktionspointer gültig ist (NOT-NULL)  
  Vector<CommandHandler_Map::key_type> allKeys = g_CommandHandler_Map.allKeys();
    for(int i=0; i < allKeys.size(); i++)
      if(allKeys[i] == command)
        if(g_CommandHandler_Map[ allKeys[i] ])
          return g_CommandHandler_Map[ allKeys[i] ](command, args_params);
       
   return;
}

// Globale Instanz
CommandHandler_Map& CommandHandler()
{
  return g_CommandHandler_Map; 
}
