#include "EVSE.h"

void usb_init()
{
  // Wartet und startet die USB-Verbindung
  Serial.begin(19200);
  while(!Serial); 
}

void usb_serialEvent()
{
  static char szBuffer[256] = {0};
  static size_t BufferOffset = 0;
  
  if(Serial.available() > 0)
  {
     const char ch = Serial.read();
    
    // /n, /r
    if(ch == 13 || ch == 10)
    {
      if(strlen(szBuffer) > 0)
      {
        // Ausführen von onCommand, wenn \n bzw. \r empfangen wurde
        // Senden des Rückgabewertes der Funktion an Sender
        Serial.println( usb_onCommand(szBuffer) );
      
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

int usb_onCommand(const char *szStr)
{
  // Kleine Test-Beispiele....
  
  if(!strcmp(szStr, "init"))
  {
    digitalWrite(13, HIGH);
    return 1;
  }
  
  else if(!strcmp(szStr, "exit"))
  {
    digitalWrite(13, LOW);
    return 1;
  }
  else if(!strcmp(szStr, "enable PWM"))
  {
    setPWM(50);
    return 1;
  }
  else if(!strcmp(szStr, "disable PWM"))
  {
    setPWM(0);
    return 1;
  }
  else if(!strcmp(szStr, "get PWM"))
  {
     return getPWM();
  }
  
  // -1, wenn der Befehl nicht vorhanden ist
  return -1;
}
