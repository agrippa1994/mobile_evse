#include <Arduino.h>
#include "USB.h"

__usbHandler g_pFunc = 0;

//! Wird aufgerufen, falls eine Nachricht von der Visualisierung angekommen ist.
void usb_onCommand(const char *szStr)
{
	if(g_pFunc)
		g_pFunc(szStr);
}

//! Initialisieren des USB-Ports
//! @param pFunc Funktionszeiger zu einer Callback-Funktion
void usb_init(__usbHandler pFunc)
{
	g_pFunc = pFunc;

	// Wartet und startet die USB-Verbindung
	Serial.begin(115200);
	while(!Serial); 
}

//! Verarbeiten der eingehenden Bytes vom USB-Port
void usb_serialEvent()
{
	static char szBuffer[256] = {0};
	static size_t BufferOffset = 0;

	if(Serial.available() > 0)
	{
		const char ch = Serial.read();

		// /n, /r
		if(ch == '\n' || ch == '\r')
		{
			if(strlen(szBuffer) > 0)
			{
				// Ausführen von onCommand, wenn \n bzw. \r empfangen wurde
				usb_onCommand(szBuffer);

				// Entleeren des Buffers, setzen des Offsets auf 0
				memset(szBuffer, 0, sizeof(szBuffer));
				BufferOffset = 0;
			}
		} 
		else
		{
			// Den Buchstaben im Buffer speichern, den Offset erhöhen
			szBuffer[BufferOffset] = ch;
			BufferOffset ++; 
		}
	} 
}

//! Interrupt-Timer des Arduinos auf dem RX-Pin
void serialEvent()
{
	usb_serialEvent();
}
