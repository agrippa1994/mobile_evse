#include "EVSE.h"
#include "Vector.h"
#include "Map.h"
#include "String.h"
#include "CommandHandler.h"
#include "StateManager.h"

void startLoading(const String& command, String_Map & args)
{
  if(args.size() == 0)
    return;
  
  if(!args.isset("--current") || !args.isset("--phases"))
    return;
  
  return;  
}

// Setup wird beim initialisieren des Programmes aufgerufen
// Hier werden unter anderem alle Objekte initialisiert, aber nicht konstruiert
void setup()
{
  pwm_init();
  usb_init(); 
  CommandHandler_init(); 
  statemanager_init();
  
  CommandHandler()["startloading"] = startLoading;
}

// loop() wird in einer for(;;) - Schleife unendlich lange aufgerufen
void loop()
{
  statemanager_update();
  
  static unsigned long last = millis();
  unsigned long current = millis();
  
  if(last + 300 > current)
    return;
  
  last = current;
  
  String data="state:";
  data += (getEVSEState() + '0');
  
  data += "\r\n";
  Serial.print(data);
}

// serialEvent() ist der Interrupt-Handler des Serialports
// Es wird hier verwendet, um Daten direkt beim Empfang zu lesen
void serialEvent()
{
  usb_serialEvent();
}
