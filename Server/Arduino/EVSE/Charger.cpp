#include "Charger.h"
#include "Pins.h"

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
  g_chargingTime = millis();
}

void disableCharging()
{
  digitalWrite(PIN_RELAY, HIGH);
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

