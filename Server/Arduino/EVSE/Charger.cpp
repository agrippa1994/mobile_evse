#include "Timer.h"

#include "Charger.h"
#include "Pins.h"
#include "PWM.h"
#include "EVSE.h"

// aktuelle Ladezeit in ms
unsigned long g_chargingTime = 0;

// Ladeinidikator
bool g_isLoading = false;

// Timer, welcher zum Stoppen der Ladung verwendet wird
Timer g_stopTimer;


void charger_stopChargingHandler(Timer *t)
{
  
}

void charger_init()
{
  pinMode(PIN_RELAY,OUTPUT);
  
  // Deaktivieren der Ladung
  disableCharging();
  disableRelay();
  
  g_chargingTime = 0;
}

void charger_update()
{
  g_stopTimer.update();  
}

void enableCharging(int amps)
{
  setPWMAmpere(amps);
}

void disableCharging()
{
  if(g_isLoading) // Anfrage zum Stoppen
  {
    g_stopTimer.start(3000, charger_stopChargingHandler, false); 
  }
  // konstante Ausgangsspannung am CP-Ausgang
  setPWM(249);
}

void enableRelay()
{
  g_currentLoadingCurrent = g_requestLoadingCurrent;
  g_chargingTime = millis();
  g_isLoading = true;
  
  digitalWrite(PIN_RELAY, HIGH);
}

void disableRelay()
{
  g_currentLoadingCurrent = 0;
  g_isLoading = false;
  
  digitalWrite(PIN_RELAY, LOW);
}

unsigned long chargingTime()
{
  if(!g_isLoading)
    return 0;
    
  return millis() - g_chargingTime;
}

bool isLoading()
{
  return g_isLoading;
}

