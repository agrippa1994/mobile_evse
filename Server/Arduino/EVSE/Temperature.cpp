#include <Arduino.h>
#include "Temperature.h"
#include "Pins.h"

double readTemperature()
{
	static int readCycles = 3;
	int readedValues[readCycles];

	for(int i=0; i<readCycles; i++)
		readedValues[i] = analogRead(PIN_TEMPERATURE);

	int sum = 0;
	for(int i=0;i<readCycles; i++)
		sum += readedValues[i];

	int voltageInt = sum / readCycles;

	return (double)voltageInt * 0.49;
}