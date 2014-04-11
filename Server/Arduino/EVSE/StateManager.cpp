#include "StateManager.h"
#include "Pins.h"
#include "EVSE.h"

#include <Arduino.h>

// Gibt die Größe eines Arrays zurück
template<typename T, int s> int arraySize(T (&) [s]) { return s; }

// Status
eState g_State = state_None;

// Callback für Statusänderungen
__stateChange g_stateChange = 0;

// Toleranz für das Einlesen der Spannungen
const int VOLTAGE_DROP_TOLERANCE = 50;


// TODO: richtige Werte suchen
const int StateVoltage[] = 
{
	0,		// state_None
	1000,	// state_A
	730, 	// state_B
	460,	// state_C
	200,	// state_D
	0,		// state_E
	0		// state_F
};

// Liest den Spannungsabfall, bei Force wird der Force-Wert zurückgegeben
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

// Initialisieren des Status-Managers
void statemanager_init(__stateChange pFunc)
{
	g_stateChange = pFunc;
}

// Update des Status-Managers
void statemanager_update()
{
	// Sichern des Statuswertes
	eState oldState = g_State;

	// Lesen der Spannung
	int high = readVoltageDrop();

	// Ändern des Statuswertes
	for(int i=0;i<arraySize(StateVoltage); i++)
		if(high >= StateVoltage[i] && high <= (StateVoltage[i] + VOLTAGE_DROP_TOLERANCE))
			g_State = (eState) i;

	// Aufruf des Callbacks, falls eine Änderung war    
	eState newState = g_State;
	if(newState != oldState && g_stateChange)
		g_stateChange(oldState, newState);
}

// Gibt den aktuellen Status zurück
eState getEVSEState()
{
	return g_State;
}
