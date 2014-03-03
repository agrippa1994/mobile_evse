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

int updateSpeed = 100;

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

void config(const String& command, String_Map & args)
{
  if(args.isset("--updatespeed"))
  {
    int val = stringToInt(args["--updatespeed"]);
    
    updateSpeed = val; 
  }
  
  if(args.isset("--digitalWrite") && args.isset("--value"))
  {
    int pin = stringToInt(args["--digitalWrite"]);
    if(pin <= 1 || pin > 13)
      return;
      
    int value = stringToInt(args["--value"]);
    if(value == 0 || value == 1)
    {
       digitalWrite(pin, value);
    }    
  }
  
  if(args.isset("--pwm"))
  {
    int val = stringToInt(args["--pwm"]);

    setPWM(val); 
  }
  
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
  CommandHandler()["config"] = config;
}

// loop() wird in einer for(;;) - Schleife unendlich lange aufgerufen
void loop()
{
  static unsigned long last_loop_time = millis();
  
  statemanager_update();

  static unsigned long last_time = millis();
  unsigned long now = millis();
  
  unsigned long current_loop_time = millis();
  unsigned long loop_time = current_loop_time-last_loop_time;
  
  if(last_time + updateSpeed >= now)
    return;
    
  last_time = now;
  
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
  
  data += " updateSpeed:";
  data += updateSpeed;
  
  data += " PWM:";
  data += getPWM();
  
  for(int i=A0, u = 0; i <= A5; i++, u++)
  {
    data += " A";
    data += u;
    data += ":";
    
    data += analogRead(i);
  }
  
  for(int i=0; i <= 13; i++)
  {
    data += " D";
    data += i;
    data += ":";
   
   data += digitalRead(i); 
  }
  
  data += " loopTime:";
  data += loop_time;

  data += "\r\n";
  Serial.print(data);
  
  last_loop_time = millis();
}

// serialEvent() ist der Interrupt-Handler des Serialports
// Es wird hier verwendet, um Daten direkt beim Empfang zu lesen
void serialEvent()
{
  usb_serialEvent();
}
