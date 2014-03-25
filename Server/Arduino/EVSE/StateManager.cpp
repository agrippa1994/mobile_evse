#include "StateManager.h"
#include "Pins.h"
#include "EVSE.h"

#include <Arduino.h>

template<typename T, int s> int arraySize(T (&) [s]) { return s; }

eState g_State = state_None;
__stateChange g_stateChange = 0;
const int VOLTAGE_DROP_TOLERANCE = 50;


// TODO: richtige Werte suchen
const int StateVoltage[] = {
  0,    // state_None
  875,  // state_A
  780,  // state_B
  690,  // state_C
  390,  // state_D
  512,  // state_E
  0     // state_F
};

int readVoltageDrop()
{  
  if(g_stateForce)
    return StateVoltage[g_stateForceState];
  
  int  val = 0, high = 0;
  for(int i=0; i<100; i++)
    if((val = analogRead(PIN_CP_VOLTAGE_DROP)) > high)
      high = val;
      
  return high;
}

void statemanager_init(__stateChange pFunc)
{
  g_stateChange = pFunc;
}

void statemanager_update()
{
  eState oldState = g_State;
  int high = readVoltageDrop();
  
  // Ändern des Statuswertes
  for(int i=0;i<arraySize(StateVoltage); i++)
    if(high >= StateVoltage[i] && high <= (StateVoltage[i] + VOLTAGE_DROP_TOLERANCE))
      g_State = (eState) i;
      
  // Aufruf des Callbacks    
  eState newState = g_State;
  if(newState != oldState && g_stateChange)
    g_stateChange(oldState, newState);
}

eState getEVSEState()
{
  return g_State;
}
