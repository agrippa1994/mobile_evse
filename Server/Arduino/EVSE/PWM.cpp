#include "EVSE.h"
#include "Pins.h"
#include "PWM.h"

void pwm_init()
{
	// Pin 10 wird für die PWM verwendet
	pinMode(PIN_PWM, OUTPUT);
}

void setPWM(int value)
{
	// Value stimmt nicht
	if(value < 0 || value > 249)
		return;

	if(value == PWM_POS_12V)
	{
		digitalWrite(PIN_PWM, LOW);
	}
	else if (value == PWM_NEG_12V)
	{
		digitalWrite(PIN_PWM, HIGH); 
	}
	else
	{
		// Erstellen einer Pulsweitenmodulation auf dem digitalen Pin 10
		TCCR1A = _BV(COM1A0) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
		TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10);

		// 0 - 249 ist das Tastverhältnis
		// Um das Tastverhältnis der Pulsweitenmodulation zu verändern,
		// muss das Register OCR1B von 0 - 249 gesetzt werden

		OCR1A = 249;   // 1kHz
	}

	OCR1B = value; 
}

// Gibt das Tastverhältnis der PWM zurück (0-249)
int getPWM()
{
	return OCR1B; 
}

bool isPWMEnabled()
{
	return getPWM() != 0; 
}

void setPWMAmpere(int ampere)
{
	if(ampere < 6 || ampere > 51)
		return;

	float I = (float)ampere;

	float dutyCycle = I / 0.6; // Norm

	// Skalieren auf 0 - 249
	float dutyForPWM = (dutyCycle * 2.49);

	setPWM(249 - (int)dutyForPWM);
}

