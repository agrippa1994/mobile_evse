#include "StateManager.h"
#include "Pins.h"
#include "EVSE.h"

#include <Arduino.h>

//! Gibt die Größe eines Arrays zurück
template<typename T, int s> int arraySize(T (&) [s]) { return s; }

//! Status
eState g_State = state_F;

//! Callback für Statusänderungen
__stateChange g_stateChange = 0;

//! Toleranz für das Einlesen der Spannungen
const int VOLTAGE_DROP_TOLERANCE = 60;

//! Spannungen, welche den Status bestimmen.
const int StateVoltage[] = 
{
	770,	///< Spannungsabfall für Status A.
	680, 	///< Spannungsabfall für Status B.
	600,	///< Spannungsabfall für Status C.
	510,	///< Spannungsabfall für Status D.
	420,	///< Spannungsabfall für Status E.
	1024	///< Spannungsabfall für Status F (kann nie erreicht werden!).
};

//! Liest den Spannungsabfall, bei Force wird der Force-Wert zurückgegeben
//! @return Gibt den Spannungsabfall zurück.
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

//! Initialisieren des Status-Managers
//! @param pFunc Funktionszeiger dur einer Callback-Funktion
//! @see __stateChange()
void statemanager_init(__stateChange pFunc)
{
	g_stateChange = pFunc;
}

//! Update des Status-Managers
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
		g_stateChange(newState);
}

//! @return Gibt den aktuellen Status des Fahrzeuges zurück
//! @see eState()
eState getEVSEState()
{
	return g_State;
}
