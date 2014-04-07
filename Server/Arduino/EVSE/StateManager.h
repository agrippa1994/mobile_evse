#ifndef STATEMANAGER_H
#define STATEMANAGER_H

/*
    Status-Codes:
      A1  Nichts angeschlossen, 12V statisch 
      A2  Nichts angeschlossen, 12V PWM
    
      B1  Angeschlossen, 9V statisch
      B2  Angeschlossen, 9V PWM
    
      C1  Bereit, 6V statisch (ohne Lüfter)
      C2  Bereit, 6V PWM (ohne Lüfter)
    
      D1  Bereit, 3V statisch (mit Lüfter)
      D2  Bereit, 3V PWM (mit Lüfter)
    
      E  Entweder Spannungsausfall am EVSE oder Kurzschloss zwischen CP PE (0V)
      F  EVSE nicht verfügbar (-12V statisch)
      
    Wie wird der Status bestimmt?
      Das Einlesen erfolgt über Pin A0 mit einer Spannung von 0-5V (-12V bis +12V in 1024 Schritten)
      
    Stromstärke wird über PWM mitgeteilt
    
    1 -> Wir können keinen Strom liefern
    2 -> Wir können Strom liefern
*/

enum eState
{
  state_None = 0,
  state_A,
  state_B,
  state_C,
  state_D,
  state_E,
  state_F
};

// Callback-Typendefinition
typedef void (* __stateChange)(eState, eState);

// Initialisieren und Update des State-Managers
void statemanager_init(__stateChange pFunc);
void statemanager_update();

// Aktueller Status
eState getEVSEState();

#endif
