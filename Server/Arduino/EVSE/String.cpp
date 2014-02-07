#include "String.h"

Vector<String> split(const String & s, const char *delims)
{
  Vector<String> tokens; 
  
  if(s.length() == 0)
    return tokens;
    
  size_t bufLen = s.length() + 1;  
  char *str = new char[bufLen];
  memset(str, 0, bufLen);
  
  s.toCharArray(str, bufLen);
  
  char *pch = strtok(str, delims);
  
  while(pch != NULL)
  {
    tokens += pch; 
    pch = strtok(NULL, delims);
  }

  delete [] str;
  
  return tokens;
}
