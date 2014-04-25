#include "Timer.h"
#include "Charger.h"
#include "Pins.h"
#include "PWM.h"
#include "EVSE.h"
#include "StateManager.h"

// aktuelle Ladezeit in ms
unsigned long g_chargingTime = 0;

// Ladeinidikator
bool g_isLoading = false;

// Timer, welcher zum Stoppen der Ladung verwendet wird
Timer g_stopTimer;


// Timer-Callback welcher aufgerufen wird, falls die Ladung deaktiviert wird
void charger_stopChargingHandler(Timer *t)
{
	eState state = getEVSEState();
	if(state != state_C && state != state_D)
		return;

	disableRelay();
}

// Initialisieren des Ladecontrollers
void charger_init()
{
	pinMode(PIN_RELAY,OUTPUT);

	// Deaktivieren der Ladung
	disableCharging();
	disableRelay();

	g_chargingTime = 0;
}

// Updatedes Ladecontrollers
void charger_update()
{
	g_stopTimer.update();  
}

// Aktivieren der PWM
void enableCharging(int amps)
{
	setPWMAmpere(amps);
}

// Deaktivieren der PWM
void disableCharging()
{
	if(g_isLoading) // Anfrage zum Stoppen
	{
		g_stopTimer.start(4000, charger_stopChargingHandler, false); 
	}

	// konstante Ausgangsspannung am CP-Ausgang
	setPWM(PWM_POS_12V);
}

// Aktivieren des Relais
void enableRelay()
{
	g_stopTimer.stop();
	
	g_currentLoadingCurrent = g_requestLoadingCurrent;
	g_chargingTime = millis();
	g_isLoading = true;

	digitalWrite(PIN_RELAY, HIGH);
}

// Deaktivieren des Relais
void disableRelay()
{
	// Stoppen des Timers
	g_stopTimer.stop();

	g_currentLoadingCurrent = 0;
	g_isLoading = false;

	digitalWrite(PIN_RELAY, LOW);
}

// Abändern des Ladestromes
void changeLoadingCurrent(int amps)
{
	if((amps >= 6 || amps <= 18) && isLoading())
	{
		g_currentLoadingCurrent = amps;
		setPWMAmpere(amps);
	}		
}

// Aktuelle Ladezeit
unsigned long chargingTime()
{
	if(!g_isLoading)
		return 0;

	return millis() - g_chargingTime;
}

// Ist Ladung aktiv?
bool isLoading()
{
	return g_isLoading;
}

