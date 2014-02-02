#include "EVSE.h"

void usb_init()
{
  // Wartet und startet die USB-Verbindung
  Serial.begin(115200);
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
    if(ch == '\n' || ch == '\r')
    {
      if(strlen(szBuffer) > 0)
      {
        // Ausführen von onCommand, wenn \n bzw. \r empfangen wurde
        // Senden des Rückgabewertes der Funktion an Sender
        String szStr = szBuffer;
        Serial.println( usb_onCommand(szStr) );
      
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

String usb_onCommand(const String & szStr)
{
  return "unknown command";
}
