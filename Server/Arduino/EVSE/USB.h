#ifndef SERIAL_H
#define SERIAL_H

// Wird vom Interrupt-Handler aufgerufen
void usb_serialEvent();

// Initialisieren der USB-Kommunikation
void usb_init();

// Verarbeitung der Befehle geschieht hier
void usb_onCommand(const String & str);

#endif
