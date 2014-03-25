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
  g_chargingTime = millis();
  
  setPWM(amps * 2);
  
  g_isLoading = true;
}

void disableCharging()
{
  setPWM(0);
  g_isLoading = false;
}

void enableRelay()
{
  digitalWrite(PIN_RELAY, HIGH);
}

void disableRelay()
{
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

