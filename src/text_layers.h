#ifndef TEXT_LAYERS_H
#define TEXT_LAYERS_H
#include <pebble.h>

//Initialize textlayer
void init_text_layer(TextLayer* text_layer, Layer* window_layer, const char* font);
//create text layers
void create_text_layers(Window *window);
//Resize received message to fit the Pebble screen
void resize_message(char* message, TextLayer* message_layer);
//destroy text layers
void destroy_text_layers();
//Toggle textlayers
void hide_text_layer(TextLayer* text_layer, bool on_off);

#endif