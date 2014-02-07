#include "EVSE.h"
#include "vector.h"
#include "map.h"

// Setup wird beim initialisieren des Programmes aufgerufen
// Hier werden unter anderem alle Objekte initialisiert, aber nicht konstruiert


void setup()
{
  pwm_init();
  usb_init(); 
}

// loop() wird in einer for(;;) - Schleife unendlich lange aufgerufen

typedef Map<String, String> String_Map;

String_Map getValues()
{
  String_Map m;
  
  for(int i=0; i < 10; i++)
  {
    String s1 = "Param ";
    s1 += i;
   
   String s2 = "Value ";
    s2 += i;
   
   m[s1] = s2; 
  }
  return m;
}

void loop()
{
  typedef Map<String, String> String_Map;
  
  String_Map m = getValues();;
  
  Vector<String_Map::key_type> keys = m.allKeys();
  Serial.print("Groeße: ");
  Serial.println(keys.size());
  
  for(int i=0;i<keys.size(); i++)
  {
     Serial.print("Key: ");
     Serial.print(keys[i]);
     Serial.print(" Value: ");
     Serial.print(m[ keys[i] ]);
     Serial.println();  
  }
}

// serialEvent() ist der Interrupt-Handler des Serialports
// Es wird hier verwendet, um Daten direkt beim Empfang zu lesen
void serialEvent()
{
  usb_serialEvent();
}
