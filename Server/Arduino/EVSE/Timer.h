#ifndef TIMER_H
#define TIMER_H

// Timer für den Arduino

class Timer
{
	typedef void (* timerHandler) (class Timer *);

	unsigned long	_startTime;
	unsigned long	_period;
	bool			_repeat;
	bool			_isActive;
	timerHandler	_handler;

public:
	// Startet den Timer
	void start(unsigned long ms, timerHandler t, bool repeat = false);

	// Stoppt den Timer
	void stop();

	// Setzen des Timer-Intervalls
	void setTimerInterval(unsigned long ms);

	// Muss aufgerufen werden, um den Timer auszuführen
	void update();
};

#endif
