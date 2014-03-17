#ifndef CHARGER_H
#define CHARGER_H

void charger_init();
void charger_update();

void enableCharging(int amps);
void disableCharging();

unsigned long chargingTime();

bool isLoading();

#endif
