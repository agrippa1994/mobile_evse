#ifndef PWM_H
#define PWM_H

// Initialisieren der PWM
void pwm_init();

// Setzen der PWM
void setPWM(int value);

// Auslesen der PWM
int  getPWM();
  
bool isPWMEnabled();  

#endif
