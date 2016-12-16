#ifndef ANIMATION_H
#define ANIMATION_H
#include <pebble.h>

PropertyAnimation *menu_animation;

//reset menu attributes, used on incoming message
void reset_menu_attributes();
//Move menu layer up or down the screen
void move_menu(int direction, MenuLayer* menu_layer, int index);
void move_text(int direction, TextLayer* text_layer, int index);
void reset_text_layer_position(TextLayer* text_layer);
#endif