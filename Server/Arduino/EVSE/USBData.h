#ifndef USBDATA_H
#define USBDATA_H

#include <Arduino.h>

// Diese Funktion wird für das Formatieren der Nachricht, welche per USB versendet wird, verwendet.
template<typename T, typename U> void addValueToString(String & s, T t, U u, bool First = false)
{
  if(!First)
    s += " ";
  
  s += t;
  s += ":";
  s += u; 
}

#endif
