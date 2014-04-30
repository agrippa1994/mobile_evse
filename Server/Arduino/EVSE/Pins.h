#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

// digitale Pins
const int PIN_RELAY = 11; 	///< Globale Definition des Relais-Pin
const int PIN_PWM = 10;		///< PWM-Pin

// analoge Pins
const int PIN_CP_VOLTAGE_DROP = A0;	///< Analoger Pin, welcher den Spannungsabfall am CP-Kreis misst.
const int PIN_TEMPERATURE = A1;		///< Analoger Pin, welcher Spannungsabfall am Temperatursensor misst.

#endif
