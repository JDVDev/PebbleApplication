#ifndef CLOCK_H
#define CLOCK_H
#include <pebble.h>

TextLayer *clock_layer;

void draw_clock(Window* window);
void hide_clock(bool on_off);
void tick_handler(struct tm *tick_time, TimeUnits units_changed);
void update_time();

#endif
