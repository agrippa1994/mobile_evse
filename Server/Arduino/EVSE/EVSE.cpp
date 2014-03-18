#include "EVSE.h"
#include "String.h"
#include "StateManager.h"
#include "USBData.h"
#include "Charger.h"
#include "Pins.h"

#include <string.h>
#include <stdio.h>

int requestLoading = 0;
int requestLoadingCurrent = 0;
int requestStopLoading = 0;

int updateSpeed = 200;


// Verarbeiten der Befehle, welche per USB empfangen werden
// Mögliche Befehle
//   startloading --current [STROM]
//   stoploading
//   config --pwm [PWM]
//   config --digitalWrite [PIN] --value [0/1]
//   config --updatespeed [SPEED in ms]
void commandHandler(const char *sz)
{
  if(strstr(sz, "startloading --current") != 0)
  {
    int current = 0;
    if(sscanf(sz, "startloading --current %d", &current) == 1)
    {
      requestLoading = 1;
      requestStopLoading = 0; 
      requestLoadingCurrent = current;
    }
    return;
  }
  
  else if(strstr(sz, "stoploading") != 0)
  {
    requestLoading = 0;
    requestLoadingCurrent = 0;
    return;
  }
  
  else if(strstr(sz, "config") != 0)
  {
    if(strstr(sz, "config --pwm") != 0)
    {
      int pwm = 0;
      if(sscanf(sz, "config --pwm %d", &pwm) == 1)
      {
        setPWM(pwm);
      } 
    }
    else if(strstr(sz, "config --digitalWrite") != 0)
    {
      int pin = 0, val = 0;
      if(sscanf(sz, "config --digitalWrite %d --value %d", &pin, &val) == 2)
      {
        if(pin <= 1 || pin > 13)
          return;
          
        if(val == 0 || val == 1)
          digitalWrite(pin, val);
      } 
    }
    else if(strstr(sz, "config --updatespeed") != 0)
    {
      int speed = 0;
      if(sscanf(sz, "config --updatespeed %d", &speed) == 1)
      {
        if(speed >= 200)
          updateSpeed = speed;
      } 
    }
  }
}

// Callback für den genormten EVSE Status
// Wird aufgerufen, falls eine Änderung erfolgt
void evseStateChange(eState _old, eState _new)
{
  if(_new == _old)
    return;
    
  switch(_new)
  {
  case state_B:
      enableCharging(requestLoadingCurrent);
    break;  
  case state_C:
  case state_D:
    
    break;
  default:
      disableCharging();
    break;  
  }
}

void send_usb_data()
{
  String data;
    
  addValueToString(data, "state", getEVSEState(), true);
  addValueToString(data, "requestLoading", requestLoading);
  addValueToString(data, "requestLoadingCurrent", requestLoadingCurrent);
  addValueToString(data, "requestStopLoading", requestStopLoading);
  addValueToString(data, "isLoading", isLoading());
  addValueToString(data, "updateSpeed", updateSpeed);
  addValueToString(data, "PWM", getPWM());
  addValueToString(data, "temperature", (float)(analogRead(PIN_TEMPERATURE) * 0.0049 * 100));
  
  Serial.println(data);
}

// Setup wird beim initialisieren des Programmes aufgerufen
// Hier werden unter anderem alle Objekte initialisiert, aber nicht konstruiert
void setup()
{
  pwm_init();
  charger_init();
  
  statemanager_init(evseStateChange);
  usb_init(commandHandler); 
}

// loop() wird in einer for(;;) - Schleife unendlich lange aufgerufen
void loop()
{
  // Update
  {
    statemanager_update();
    charger_update();
  }
  
  // Senden der Daten
  {
    static unsigned long last_time = millis();
    unsigned long now = millis();
    
    if(last_time + updateSpeed >= now)
      return;
      
    send_usb_data();  
    last_time = now;
  }
}
