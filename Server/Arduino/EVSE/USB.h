#ifndef SERIAL_H
#define SERIAL_H

// Wird vom Interrupt-Handler aufgerufen
void usb_serialEvent();

// Initialisieren der USB-Kommunikation
void usb_init();

// Verarbeitung der Befehle geschieht hier
int  usb_onCommand(const char *szStr);

#endif
