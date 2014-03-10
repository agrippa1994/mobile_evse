#include "EVSE.h"
#include "String.h"
#include "StateManager.h"

#include <string.h>
#include <stdio.h>

int requestLoading = 0;
int requestLoadingCurrent = 0;
int requestStopLoading = 0;

int isLoading = 0;
int isStopped = 1;

int updateSpeed = 200;

// Diese Funktion wird für das Formatieren der Nachricht, welche per USB versendet wird, verwendet.
template<typename T, typename U> void addValueToString(String & s, T t, U u, bool First = false)
{
  if(!First)
    s += " ";
  
  s += t;
  s += ":";
  s += u; 
}

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
  
}

// Setup wird beim initialisieren des Programmes aufgerufen
// Hier werden unter anderem alle Objekte initialisiert, aber nicht konstruiert
void setup()
{
  pwm_init();
  
  statemanager_init(evseStateChange);
  usb_init(commandHandler); 
}

// loop() wird in einer for(;;) - Schleife unendlich lange aufgerufen
void loop()
{
  // Update
  {
    statemanager_update();
  }
  
  // Senden der Daten
  {
    static unsigned long last_time = millis();
    unsigned long now = millis();
    
    if(last_time + updateSpeed >= now)
      return;
      
    last_time = now;
    
    String data;
    
    addValueToString(data, "state", getEVSEState(), true);
    addValueToString(data, "requestLoading", requestLoading);
    addValueToString(data, "requestLoadingCurrent", requestLoadingCurrent);
    addValueToString(data, "requestStopLoading", requestStopLoading);
    addValueToString(data, "isLoading", isLoading);
    addValueToString(data, "isStopped", isStopped);
    addValueToString(data, "updateSpeed", updateSpeed);
    addValueToString(data, "PWM", getPWM());
    addValueToString(data, "temperature", (float)(analogRead(A0) * 0.0049 * 100));
    
    // Senden der analogen Daten
    for(int i=A0, u = 0; i <= A5; i++, u++) addValueToString(data, String( String("A") + String(u)), analogRead(i));
    
    // Senden der digitalen Daten
    for(int i=0; i <= 13; i++)  addValueToString(data, String( String("D") + String(i)), digitalRead(i));
    
    Serial.println(data);
  }
}
