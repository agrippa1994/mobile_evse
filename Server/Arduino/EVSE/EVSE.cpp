#include "EVSE.h"
#include "String.h"
#include "StateManager.h"
#include "USBData.h"
#include "Charger.h"
#include "Pins.h"

#include <string.h>
#include <stdio.h>

int g_requestLoading = 0;
int g_requestLoadingCurrent = 0;
int g_requestStopLoading = 0;

int g_updateSpeed = 200;

int g_stateForce = 0;
int g_stateForceState = 0;

// Verarbeiten der Befehle, welche per USB empfangen werden
// Mögliche Befehle
//   startloading --current [STROM]
//   stoploading
//   config --pwm [PWM]
//   config --digitalWrite [PIN] --value [0/1]
//   config --updatespeed [SPEED in ms]
//   config --force [Index nach eState]
//   config --force disable
void commandHandler(const char *sz)
{
  if(strstr(sz, "startloading --current") != 0)
  {
    if(getEVSEState() != state_B)
      return;
      
    int current = 0;
    if(sscanf(sz, "startloading --current %d", &current) == 1)
    {
      g_requestLoading = 1;
      g_requestStopLoading = 0; 
      g_requestLoadingCurrent = current;
      enableCharging(g_requestLoadingCurrent);
    }
    return;
  }
  
  else if(strstr(sz, "stoploading") != 0)
  {
    eState state = getEVSEState();
    if(!(state == state_C || state == state_D))
      return;
      
    g_requestLoading = 0;
    g_requestLoadingCurrent = 0;
    disableCharging();
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
          g_updateSpeed = speed;
      } 
    }
    else if(strstr(sz, "config --force") != 0)
    {
      char param[28] = { 0 };
      if(sscanf(sz, "config --force %s", param))
      {
        if(strcmp(param, "disable") == 0)
        {
          g_stateForce = 0;
          g_stateForceState = 0;
          return;
        }
      
        int idx = atoi(param);     
        if(idx < 0 || idx > 6)
          return;
        
        g_stateForceState = idx;
        g_stateForce = 1;
      }
    }
  }
}

// Callback für den genormten EVSE Status
// Wird aufgerufen, falls eine Änderung erfolgt
void evseStateChange(eState oldState, eState newState)
{
  if(newState == oldState)
    return;
  
  // Sequenz 1.1  
  if(((oldState == state_A || newState == state_B) || newState == state_A) && !isPWMEnabled())
  {
    
  } 
  // Sequence 1.2 (Plug-in (w/o S2)) 
  else if((oldState == state_A && (newState == state_C || newState == state_D)) && !isPWMEnabled())
  {
     
  }
  
  // Sequence 2.1 (Uplug at state Bx)
  else if(  (oldState == state_B && isPWMEnabled()) && (newState == state_A && isPWMEnabled())
         && (oldState == state_B && !isPWMEnabled()) && (newState == state_A && !isPWMEnabled()))
  {
    
  }
  // Sequence 2.2 (Unplug during charging)
  else if(  ((oldState == state_C && isPWMEnabled()) || (newState == state_D && isPWMEnabled())) && newState == state_A)
  {
    
  }
  else if( (oldState == state_A && isPWMEnabled()) && (newState == state_A && !isPWMEnabled()))
  {
    
  }
  
  // Sequence 3.1 (EVSE Power available (state B))
  else if( (oldState == state_B && !isPWMEnabled()) && (newState == state_B && isPWMEnabled()))
  {
    
  }
  // Sequence 3.2 (EVSE Power available (state C))
  else if( (oldState == state_C && !isPWMEnabled()) && (newState == state_C && isPWMEnabled()))
  {
    
  }
  
  // Sequence 4 (EV ready to charge)
  else if( (oldState == state_B) && (newState == state_C || newState == state_D) && isPWMEnabled())
  {
    
  }
  
  // Sequences between 4 and 7 aren't needed
  
  // Sequence 7 (EV stops the charge)
  else if((oldState == state_C || oldState == state_D) && (newState == state_B) && isPWMEnabled())
  {
    
  }
  
  
}

void send_usb_data()
{
  String data;
    
  addValueToString(data, "state", getEVSEState(), true);
  addValueToString(data, "requestLoading", g_requestLoading);
  addValueToString(data, "requestLoadingCurrent", g_requestLoadingCurrent);
  addValueToString(data, "requestStopLoading", g_requestStopLoading);
  addValueToString(data, "isLoading", isLoading());
  addValueToString(data, "updateSpeed", g_updateSpeed);
  addValueToString(data, "PWM", getPWM());
  addValueToString(data, "temperature", (double)(analogRead(PIN_TEMPERATURE) * 0.0049 * 100));
  addValueToString(data, "chargingTime", chargingTime());
  addValueToString(data, "force", g_stateForce);
  
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
    
    if(last_time + g_updateSpeed >= now)
      return;
      
    send_usb_data();  
    last_time = now;
  }
}
