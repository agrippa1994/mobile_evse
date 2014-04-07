#ifndef TIMER_H
#define TIMER_H

// Timer für den Arduino

class Timer
{
  typedef void (* timerHandler) (class Timer *);
  
  unsigned long _startTime;
  unsigned long _period;
  bool          _repeat;
  bool          _isActive;
  timerHandler  _handler;
  
public:
  void start(unsigned long ms, timerHandler t, bool repeat = false);
  void stop();
  void update();
};

#endif
