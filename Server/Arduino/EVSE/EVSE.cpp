#include "EVSE.h"
#include "vector.h"
#include "map.h"
#include "string.h"

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
  String x = "startLoad --param 1 --asd 43 --asdr -5";
  
  Vector<String> tokens = split(x, " ");
  for(int i = 0; i < tokens.size(); i++)
    Serial.println(tokens[i]);
  
}

// serialEvent() ist der Interrupt-Handler des Serialports
// Es wird hier verwendet, um Daten direkt beim Empfang zu lesen
void serialEvent()
{
  usb_serialEvent();
}
