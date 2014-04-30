#include "Timer.h"
#include "Charger.h"
#include "Pins.h"
#include "PWM.h"
#include "EVSE.h"
#include "StateManager.h"

//! aktuelle Ladezeit in ms.
unsigned long g_chargingTime = 0;

//! Ladeinidikator.
bool g_isLoading = false;

//! Timer, welcher zum Stoppen der Ladung verwendet wird.
Timer g_stopTimer;



//! Timer-Callback welcher aufgerufen wird, falls die Ladung deaktiviert wird.
//! @param t Timer-Pointer zum Objekt, welches den Callback ausgelöst hat.
void charger_stopChargingHandler(Timer *t)
{
	eState state = getEVSEState();
	if(state != state_C && state != state_D)
		return;

	disableRelay();
}

//! Initialisieren des Ladecontrollers
void charger_init()
{
	pinMode(PIN_RELAY,OUTPUT);

	// Deaktivieren der Ladung
	disableCharging();
	disableRelay();

	g_chargingTime = 0;
}

//! Updatedes Ladecontrollers
void charger_update()
{
	g_stopTimer.update();  
}

//! Aktivieren der Ladung, indem eine Pulsweitenmodulation gestartet wird.
//! @param amps Angabe der Ampere, mit der das Fahrzeug geladen werden soll.
void enableCharging(int amps)
{
	setPWMAmpere(amps);
}

//! Beenden der Ladung, indem die Pulsweitenmodulation beendet wird. Reagiert das Fahrzeug nicht auf die Stopanfrage,
//! dann wird die Ladung automatisch nach 4 Sekunden beendet. @see charger_stopChargingHandler()
void disableCharging()
{
	if(g_isLoading) // Anfrage zum Stoppen
	{
		g_stopTimer.start(4000, charger_stopChargingHandler, false); 
	}

	// konstante Ausgangsspannung am CP-Ausgang
	setPWM(PWM_POS_12V);
}

//! Aktivieren des Relais
void enableRelay()
{
	g_stopTimer.stop();
	
	g_currentLoadingCurrent = g_requestLoadingCurrent;
	g_chargingTime = millis();
	g_isLoading = true;

	digitalWrite(PIN_RELAY, HIGH);
}

//! Deaktivieren des Relais
void disableRelay()
{
	// Stoppen des Timers
	g_stopTimer.stop();

	g_currentLoadingCurrent = 0;
	g_isLoading = false;

	digitalWrite(PIN_RELAY, LOW);
}

//! Abändern des Ladestromes
//! @param amps Ladestrom in Ampere
void changeLoadingCurrent(int amps)
{
	if((amps >= 6 || amps <= 20) && isLoading())
	{
		g_currentLoadingCurrent = amps;
		setPWMAmpere(amps);
	}		
}

//! Aktuelle Ladezeit.
//! @return Gibt die aktuelle Ladezeit in ms zurück.
unsigned long chargingTime()
{
	if(!g_isLoading)
		return 0;

	return millis() - g_chargingTime;
}

//! Ist die Ladung aktiv?
//! @return Gibt zurück, ob die Ladung aktiv ist oder nicht.
bool isLoading()
{
	return g_isLoading;
}

