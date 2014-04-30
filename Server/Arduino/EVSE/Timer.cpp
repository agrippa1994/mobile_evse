#include "Timer.h"
#include <Arduino.h>

//! Startet den Timer.
//! @param ms Zeitangabe in ms
//! @param t Funktionszeiger zu einem Callback
//! @param repeat Sollte der Timer wiederholt werden?
void Timer::start(unsigned long ms, timerHandler t, bool repeat)
{
	_period = ms;
	_repeat = repeat;
	_handler = t;
	_startTime = millis();
	_isActive = true;
}

//! Stoppt den Timer
void Timer::stop()
{
	_isActive = false; 
}

//! Nachträgliches Ändern des Timer-Intervalls
void Timer::setTimerInterval(unsigned long ms)
{
	_period = ms;
}

//! Updaten des Timers
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

//! @return Ist der Timer aktiv?
bool Timer::isActive()
{
	return _isActive;
}