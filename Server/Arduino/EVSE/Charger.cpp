#include "Charger.h"
#include "Pins.h"
#include "PWM.h"

unsigned long g_chargingTime = 0;

bool g_isLoading = false;

void charger_init()
{
  pinMode(PIN_RELAY,OUTPUT);
  
  g_chargingTime = 0;
}

void charger_update()
{
  
}

void enableCharging(int amps)
{
  setPWMAmpere(amps);
}

void disableCharging()
{
  setPWMAmpere(0);
}

void enableRelay()
{
  g_chargingTime = millis();
  g_isLoading = true;
  digitalWrite(PIN_RELAY, HIGH);
}

void disableRelay()
{
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

