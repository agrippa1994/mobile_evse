#ifndef PWM_H
#define PWM_H

#define PWM_POS_12V 0
#define PWM_NEG_12V 249

// Initialisieren der PWM
void pwm_init();

// Setzen der PWM
void setPWM(int value);

// Auslesen der PWM
int  getPWM();
  
// Ist die PWm aktiv?  
bool isPWMEnabled();  

// Umrechnen des Stromes in die zugehörige Pulsweite nach Norm
void setPWMAmpere(int ampere);

#endif
