#include "EVSE.h"
#include "vector.h"


// Setup wird beim initialisieren des Programmes aufgerufen
// Hier werden unter anderem alle Objekte initialisiert, aber nicht konstruiert
void setup()
{
  pwm_init();
  usb_init();  
}

// loop() wird in einer for(;;) - Schleife unendlich lange aufgerufen
void loop()
{

}

// serialEvent() ist der Interrupt-Handler des Serialports
// Es wird hier verwendet, um Daten direkt beim Empfang zu lesen
void serialEvent()
{
  usb_serialEvent();
}
