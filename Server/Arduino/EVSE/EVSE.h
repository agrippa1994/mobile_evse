#ifndef EVSE_H
#define EVSE_H

#include <Arduino.h>
#include "PWM.h"
#include "USB.h"

extern int g_requestLoading;
extern int g_requestLoadingCurrent;
extern int g_requestStopLoading;

extern int g_updateSpeed;

extern int g_stateForce;
extern int g_stateForceState;

#endif
