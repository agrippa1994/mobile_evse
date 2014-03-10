#ifndef SERIAL_H
#define SERIAL_H

typedef void (* __usbHandler)(const char *);

// Wird vom Interrupt-Handler aufgerufen
void usb_serialEvent();

// Initialisieren der USB-Kommunikation
void usb_init(__usbHandler pFunc);

#endif
