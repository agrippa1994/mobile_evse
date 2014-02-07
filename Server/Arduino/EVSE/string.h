#ifndef STRING_H
#define STRING_H

#include <Arduino.h>
#include "vector.h"

Vector<String> split(const String & s, const char *delim);

#endif
