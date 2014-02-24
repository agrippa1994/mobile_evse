#include "StateManager.h"

#include <Arduino.h>

template<typename T, int s> int arraySize(T (&) [s]) { return s; }

eState g_State = state_None;

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

void statemanager_init()
{
  
}

void statemanager_update()
{
  int high = 0, val = 0;
  for(int i=0; i<100; i++)
    if((val = analogRead(A0)) > high)
      high = val;
  
  for(int i=0;i<arraySize(StateVoltage); i++)
    if(high >= StateVoltage[i])
      g_State = (eState) i;
}

eState getEVSEState()
{
  return g_State;
}
