#ifndef PINS_H
#define PINS_H
#include <Arduino.h>

// digitale Pins
const int PIN_RELAY = 13; // Zurzeit wird Pin 13 (Led) für das Relais angenommen
const int PIN_PWM = 10;

// analoge Pins
const int PIN_CP_VOLTAGE_DROP = A0;
const int PIN_TEMPERATURE = A1;

#endif
