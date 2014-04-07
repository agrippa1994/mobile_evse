#ifndef CHARGER_H
#define CHARGER_H

// Initialisieren bzw. Update
void charger_init();
void charger_update();

// Aktivieren / Deaktivieren der Ladung (nur die Aktivierung der PWM)
void enableCharging(int amps);
void disableCharging();

// Aktivieren / Deaktivieren des Relais
void enableRelay();
void disableRelay();

// Gibt die aktuelle Ladezeit in ms zurück
unsigned long chargingTime();

// Ist Ladung aktiv?
bool isLoading();

#endif
