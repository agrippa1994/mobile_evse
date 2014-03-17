#ifndef STRING_H
#define STRING_H

#include <Arduino.h>

String& operator += (String& in, float value);

int stringToInt(const String& c);

#endif
