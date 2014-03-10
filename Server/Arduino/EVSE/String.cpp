#include "String.h"
#include "floatToString.h"

String& operator += (String& in, float value)
{
  char szIn[32] = { 0 };
  
  in += floatToString(szIn, value, 2);;
  
  return in;
}

int stringToInt(const String& c)
{
  char buf[32] = {0};
  c.toCharArray(buf, sizeof(buf));
  return atoi(buf); 
}
