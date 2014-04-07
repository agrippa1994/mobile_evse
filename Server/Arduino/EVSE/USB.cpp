#include <Arduino.h>
#include "USB.h"

__usbHandler g_pFunc = 0;

void usb_onCommand(const char *szStr)
{
  if(g_pFunc)
    g_pFunc(szStr);
}

void usb_init(__usbHandler pFunc)
{
  g_pFunc = pFunc;
  
  // Wartet und startet die USB-Verbindung
  Serial.begin(115200);
  while(!Serial); 
}

void usb_serialEvent()
{
  static char szBuffer[512] = {0};
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

// serialEvent() ist der Interrupt-Handler des Serialports
// Es wird hier verwendet, um Daten direkt beim Empfang zu lesen
void serialEvent()
{
  usb_serialEvent();
}
