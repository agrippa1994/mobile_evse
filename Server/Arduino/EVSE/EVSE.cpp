#include "EVSE.h"
#include "Vector.h"
#include "Map.h"
#include "String.h"
#include "CommandHandler.h"
#include "StateManager.h"

int requestLoading = 0;
int requestLoadingCurrent = 0;
int requestStopLoading = 0;

int isLoading = 0;
int isStopped = 1;

int stringToInt(const String& c)
{
  char buf[32] = {0};
  c.toCharArray(buf, sizeof(buf));
  return atoi(buf); 
}

void startLoading(const String& command, String_Map & args)
{
  if(args.size() == 0)
    return;
  
  if(!args.isset("--current"))
    return;
  
  requestLoading = 1;
  requestStopLoading = 0;
  requestLoadingCurrent = stringToInt(args["--current"]);
  
  return;  
}

void stopLoading(const String& command, String_Map & args)
{
  requestLoading = 0;
  requestLoadingCurrent = 0;
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
  CommandHandler()["stoploading"] = stopLoading;
}

// loop() wird in einer for(;;) - Schleife unendlich lange aufgerufen
void loop()
{
  statemanager_update();

  String data;
  
  data +="state:";
  data += getEVSEState();

  data += " requestLoading:";
  data += requestLoading;
  
  data += " requestLoadingCurrent:";
  data += requestLoadingCurrent;
  
  data += " requestStopLoading:";
  data += requestStopLoading;
  
  data += " isLoading:";
  data += isLoading;
  
  data += " isStopped:";
  data += isStopped;
  
  data += "\r\n";
  Serial.print(data);
}

// serialEvent() ist der Interrupt-Handler des Serialports
// Es wird hier verwendet, um Daten direkt beim Empfang zu lesen
void serialEvent()
{
  usb_serialEvent();
}
