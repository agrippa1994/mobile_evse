#include "StateManager.h"
#include "Pins.h"

#include <Arduino.h>

template<typename T, int s> int arraySize(T (&) [s]) { return s; }

eState g_State = state_None;
__stateChange g_stateChange = 0;

// TODO: richtige Werte suchen
const int StateVoltage[] = {
  0,    // state_None
  875,  // state_A
  780,  // state_B
  690,  // state_C
  0,    // state_D
  512,  // state_E
  0     // state_F
};

void statemanager_init(__stateChange pFunc)
{
  g_stateChange = pFunc;
}

void statemanager_update()
{
  eState oldState = g_State;
  
  int high = 0, val = 0;
  for(int i=0; i<100; i++)
    if((val = analogRead(PIN_CP_VOLTAGE_DROP)) > high)
      high = val;
  
  for(int i=0;i<arraySize(StateVoltage); i++)
    if(high >= StateVoltage[i])
      g_State = (eState) i;
      
  eState newState = g_State;
  if(newState != oldState && g_stateChange)
    g_stateChange(oldState, newState);
}

eState getEVSEState()
{
  return g_State;
}
