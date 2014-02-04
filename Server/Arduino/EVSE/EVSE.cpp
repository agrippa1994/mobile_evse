#include "EVSE.h"
#include "vector.h"

vector<String> createArray()
{
   vector<String> x;
   x += "Hallo";
   x += " ";
   x += "Welt\r\n";
  
    return x; 
}


// Setup wird beim initialisieren des Programmes aufgerufen
// Hier werden unter anderem alle Objekte initialisiert, aber nicht konstruiert
void setup()
{
  pwm_init();
  usb_init();  
  
  delay(1000);
  
  vector<int> x;   
  x += 5;
  x += 10;
  x += 15;

  for(int i=0; i < x.size(); i++)
    Serial.println(x[i]);
    
  Serial.println(x.size());
  
  
  vector<int> y = x;
  y.remove(0);
  
  for(int i=0; i < y.size(); i++)
    Serial.println(y[i]);
   
  Serial.println(y.size());
  
  
  vector<String> str_vector = createArray();
  str_vector.remove(0);
  
  for(int i=0; i < str_vector.size(); i++)
    Serial.print(str_vector[i]);
    
  Serial.println(str_vector.size());
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
