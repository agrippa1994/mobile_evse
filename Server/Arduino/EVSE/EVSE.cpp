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

#include <string.h>
#include <stdio.h>

// Definition der globalen Variablen
int g_requestLoading = 0;
int g_requestLoadingCurrent = 0;
int g_requestStopLoading = 0;

int g_updateSpeed = 100;

int g_stateForce = 0;
int g_stateForceState = 0;

int g_currentLoadingCurrent = 0;

// USB-Timer
Timer g_usbTimer;

// Funktionsdeklarationen
void commandHandler(const char *sz);
void evseStateChange(eState oldState, eState newState);
void send_usb_timer(Timer *t);


// Setup wird beim initialisieren des Programmes aufgerufen
// Hier werden unter anderem alle Objekte initialisiert, aber nicht konstruiert
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

// loop() wird in einer for(;;) - Schleife unendlich lange aufgerufen
void loop()
{
	// Updaten des Statusmanagers und Ladecontroller
	statemanager_update();
	charger_update();

	// Update des USB-Timers
	g_usbTimer.update();  
}

// Verarbeiten der Befehle, welche per USB empfangen werden
// Mögliche Befehle
// 		startloading --current [STROM]
// 		stoploading
//   	config --pwm [PWM]
//   	config --digitalWrite [PIN] --value [0/1]
//   	config --updatespeed [SPEED in ms]
//   	config --force [Index nach eState]
//   	config --force disable
void commandHandler(const char *sz)
{
	if(strstr(sz, "startloading --current") != 0)
	{
		if(getEVSEState() != state_B)
			return;

		int current = 0;
		if(sscanf(sz, "startloading --current %d", &current) == 1)
		{
			g_requestLoading = 1;
			g_requestStopLoading = 0; 
			g_requestLoadingCurrent = current;
			enableCharging(g_requestLoadingCurrent);
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

	else if(strstr(sz, "config") != 0)
	{
		if(strstr(sz, "config --pwm") != 0)
		{
			int pwm = 0;
			if(sscanf(sz, "config --pwm %d", &pwm) == 1)
			{
				setPWM(pwm);
			} 
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
		}
		else if(strstr(sz, "config --updatespeed") != 0)
		{
			int speed = 0;
			if(sscanf(sz, "config --updatespeed %d", &speed) == 1)
			{
				if(speed >= 100)
				{
					g_updateSpeed = speed;

					// Timer-Intervall auf die Update-Geschwindigkeit setzen
					g_usbTimer.setTimerInterval(g_updateSpeed);
				}
			} 
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
				if(idx < 0 || idx > 6)
					return;

				g_stateForceState = idx;
				g_stateForce = 1;
			}
		}
	}
}

// Callback für den genormten EVSE Status
// Wird aufgerufen, falls eine Änderung erfolgt
void evseStateChange(eState oldState, eState newState)
{
	if(newState == oldState)
		return;

	// Ausfuehren aller Sequenzen laut Norm!
	if( (((oldState == state_C) || (newState == state_D)) && ((newState == state_A) || (newState == state_B)))
	||( (newState == state_A || newState == state_E || newState == state_F)))
	{
		disableCharging();
		disableRelay();
	}

	else if((oldState == state_B) && (newState == state_C || newState == state_D) && g_requestLoading)
	{
		enableCharging(g_requestLoadingCurrent);
		enableRelay();

		g_requestLoading = 0;
		g_requestLoadingCurrent = 0;
	}
}

void send_usb_timer(Timer *p)
{
	// Senden jener Daten, die für die Visualisierungen wichtig sind
	String data;
	addValueToString(data, "state", getEVSEState(), true);
	addValueToString(data, "requestLoading", g_requestLoading);
	addValueToString(data, "requestLoadingCurrent", g_requestLoadingCurrent);
	addValueToString(data, "requestStopLoading", g_requestStopLoading);
	addValueToString(data, "isLoading", isLoading());
	addValueToString(data, "updateSpeed", g_updateSpeed);
	addValueToString(data, "PWM", getPWM());
	addValueToString(data, "temperature", (double)(analogRead(PIN_TEMPERATURE) * 0.49));
	addValueToString(data, "chargingTime", chargingTime());
	addValueToString(data, "force", g_stateForce);
	addValueToString(data, "currentLoadingCurrent", g_currentLoadingCurrent);

	// Pin-Daten der analogen Pins
	for(int i=A0, u = 0; i <= A5; i ++, u++)
		addValueToString(data, String("A") + String(u), analogRead(i));

	Serial.println(data);
}
