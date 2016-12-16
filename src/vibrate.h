#ifndef VIBRATE_H
#define VIBRATE_H

#include <pebble.h>


//excecute vibration pattern
void vibrate();
//register vibration timer
void register_vibration_timer();
//cancel vibration timer
void cancel_vibration_timer();
#endif