#include "EVSE.h"
#include "Vector.h"
#include "Map.h"
#include "String.h"
#include "CommandHandler.h"

String startLoading(const String& command, String_Map & args)
{
  if(args.size() == 0)
    return "error";
  
  if(!args.isset("--current") || !args.isset("--phases"))
    return "error";
  
  String retn;
  retn += "Ladung gestartet, Strom: ";
  retn += args["--current"];

  retn += ", Phasen: ";
  retn += args["--phases"];

  return retn;  
}

// Setup wird beim initialisieren des Programmes aufgerufen
// Hier werden unter anderem alle Objekte initialisiert, aber nicht konstruiert
void setup()
{
  pwm_init();
  usb_init(); 
  CommandHandler_init(); 
  
  CommandHandler()["startloading"] = startLoading;
}

// loop() wird in einer for(;;) - Schleife unendlich lange aufgerufen
void loop()
{
  
}

// serialEvent() ist der Interrupt-Handler des Serialports
// Es wird hier verwendet, um Daten direkt beim Empfang zu lesen
void serialEvent()
{
  usb_serialEvent();
}
