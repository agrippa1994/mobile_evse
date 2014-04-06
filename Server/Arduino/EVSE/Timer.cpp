#include "Timer.h"
#include <Arduino.h>

void Timer::start(unsigned long ms, timerHandler t, bool repeat)
{
  _period = ms;
  _repeat = repeat;
  _handler = t;
  _startTime = millis();
  _isActive = true;
}

void Timer::stop()
{
  _isActive = false;
  
}
void Timer::update()
{
  if(!_isActive)
    return;
    
  if(_startTime + _period <= millis())
  {
    if(_handler)
      _handler((class Timer *)this);
    
    if(_repeat)
      _startTime = millis();
    else
      _isActive = false;
  }
}

