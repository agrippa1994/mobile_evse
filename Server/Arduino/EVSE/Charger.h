#ifndef CHARGER_H
#define CHARGER_H

// Initialisieren bzw. Update
void charger_init();
void charger_update();

// Aktivieren / Deaktivieren der Ladung (Betrifft nur PWM!)
void enableCharging(int amps);
void disableCharging();

// Aktivieren / Deaktivieren des Relais
void enableRelay();
void disableRelay();

// Ändern des Ladestromes
void changeLoadingCurrent(int amps);

// Gibt die aktuelle Ladezeit in ms zurück
unsigned long chargingTime();

// Ist Ladung aktiv?
bool isLoading();

#endif
