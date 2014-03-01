#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <Arduino.h>
#include "Map.h"

typedef Map<String, String> String_Map;
typedef Map<String, void (*)(const String&, String_Map &)> CommandHandler_Map;

void CommandHandler_init();
void CommandHandler(const String & usb_command);


CommandHandler_Map& CommandHandler();

#endif

