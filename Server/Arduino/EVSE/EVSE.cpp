#include <Arduino.h>

#include "EVSE.h"
#include "String.h"
#include "StateManager.h"
#include "USBData.h"
#include "Charger.h"
#include "Pins.h"
#include "PWM.h"
#include "USB.h"
#include "Timer.h"
#include "Temperature.h"

#include <string.h>
#include <stdio.h>

// Definition der globalen Variablen.

//! Wird 1, wenn eine Ladung angefordert worden ist.
int g_requestLoading = 0;

//! Ladestrom, welcher angefragt wurde.
int g_requestLoadingCurrent = 0;

//! Zeitintervall in ms, welches angibt, in welchem Intervall die Visualisierungsdaten geschickt werden.
int g_updateSpeed = 200;

//! Gibt an, ob der Status-Code simuliert werden soll.
int g_stateForce = 0;

//! Status, welcher geforced werden soll.
int g_stateForceState = 0;

//! Aktueller Ladestrom
int g_currentLoadingCurrent = 0;

//! USB-Timer, welcher die Daten nach dem Ablaufen des Timers die Daten an die Visualisierung sendet.
Timer g_usbTimer;

// Funktionsdeklarationen
void commandHandler(const char *sz);
void evseStateChange(eState newState);
void send_usb_timer(Timer *t);


//! Initialisieren aller Komponenten, die für das Ladecontrollerprogramm wichtig sind.
void setup()
{
	// Initialisieren der Pulsweitenmodulation
	pwm_init();

	// Initialisieren des Ladecontrollers
	charger_init();

	// Initialisieren des State-Managers
	statemanager_init(evseStateChange);

	// Initialisieren des USB-Kommunikation
	usb_init(commandHandler); 

	// Starten des USB-Timers
	g_usbTimer.start(g_updateSpeed, send_usb_timer, true);
}

//! Aktualisieren aller Komponenten.
void loop()
{
	// Updaten des Statusmanagers und Ladecontroller
	statemanager_update();
	charger_update();

	// Update des USB-Timers
	g_usbTimer.update();  
}

//! Verarbeiten der Befehle, welche per USB empfangen werden.
void commandHandler(const char *sz)
{
	if(strstr(sz, "startloading --current") != 0)
	{
		int current = 0;
		if(sscanf(sz, "startloading --current %d", &current) == 1)
		{
			if(current < 6 || current > 20)
				return;

			g_requestLoading = 1;
			g_requestLoadingCurrent = current;
			
			enableCharging(g_requestLoadingCurrent);
			evseStateChange(getEVSEState());
		}

		return;
	}

	else if(strstr(sz, "stoploading") != 0)
	{
		if(!isLoading())
			return;

		g_requestLoading = 0;
		g_requestLoadingCurrent = 0;
		disableCharging();

		return;
	}

	else if(strstr(sz, "changecurrent --current") != 0)
	{
		eState state = getEVSEState();
		if(state != state_C && state != state_D)
			return;

		int current = 0;
		if(sscanf(sz, "changecurrent --current %d", &current) == 1)
			if(current >= 6 || current <= 20)
				changeLoadingCurrent(current);

		return;
	}

	else if(strstr(sz, "config") != 0)
	{
		if(strstr(sz, "config --pwm") != 0)
		{
			int pwm = 0;
			if(sscanf(sz, "config --pwm %d", &pwm) == 1)
				setPWM(pwm);

			return;
		}
		else if(strstr(sz, "config --digitalWrite") != 0)
		{
			int pin = 0, val = 0;
			if(sscanf(sz, "config --digitalWrite %d --value %d", &pin, &val) == 2)
			{
				if(pin <= 1 || pin > 13)
					return;

				if(val == 0 || val == 1)
					digitalWrite(pin, val);
			} 

			return;
		}
		else if(strstr(sz, "config --updatespeed") != 0)
		{
			int speed = 0;
			if(sscanf(sz, "config --updatespeed %d", &speed) == 1)
			{
				if(speed >= 200)
				{
					g_updateSpeed = speed;

					// Timer-Intervall auf die Update-Geschwindigkeit setzen
					g_usbTimer.setTimerInterval(g_updateSpeed);
				}
			} 

			return;
		}
		else if(strstr(sz, "config --force") != 0)
		{
			char param[28] = { 0 };
			if(sscanf(sz, "config --force %s", param))
			{
				if(strcmp(param, "disable") == 0)
				{
					g_stateForce = 0;
					g_stateForceState = 0;
					return;
				}

				int idx = atoi(param);     
				if(idx < 0 || idx > 5)
					return;

				g_stateForceState = idx;
				g_stateForce = 1;
			}

			return;
		}
	}
}

//! Verarbeiten aller Status-Codes nach Norm.
void evseStateChange(eState newState)
{
	if(newState == state_A)
	{
		disableCharging();
		disableRelay();

		g_requestLoading = 0;
		g_requestLoadingCurrent = 0;

		return;
	}

	if(newState == state_B)
	{
		if(!g_requestLoading)
			disableCharging();
		
		disableRelay();		
		return;
	}

	if(newState == state_C || newState == state_D)
	{
		if(g_requestLoading)
			enableRelay();

		return;
	}

	if(newState == state_E || newState == state_F)
	{
		disableRelay();
		return;
	}
}

//! Senden der Daten, welche für die Visualisierung relevant sind.
void send_usb_timer(Timer *p)
{
	// Senden jener Daten, die für die Visualisierungen wichtig sind!
	String data;
	addValueToString(data, "state", getEVSEState(), true);
	addValueToString(data, "requestLoading", g_requestLoading);
	addValueToString(data, "requestLoadingCurrent", g_requestLoadingCurrent);
	addValueToString(data, "isLoading", isLoading());
	addValueToString(data, "updateSpeed", g_updateSpeed);
	addValueToString(data, "PWM", getPWM());
	addValueToString(data, "temperature", readTemperature());
	addValueToString(data, "chargingTime", chargingTime());
	addValueToString(data, "force", g_stateForce);
	addValueToString(data, "currentLoadingCurrent", g_currentLoadingCurrent);

	// Pin-Daten der analogen Pins
	for(int i=A0, u = 0; i <= A5; i ++, u++)
		addValueToString(data, String("A") + String(u), analogRead(i));

	Serial.println(data);
}
