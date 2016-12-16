#include "pebble_number.h"
#include "text_layers.h"

TextLayer* pebble_number_layer;

void draw_pebble_number(Window * window)
{	
	Layer *window_layer = window_get_root_layer(window);
	GRect window_bounds = layer_get_bounds(window_layer);

	pebble_number_layer = text_layer_create(GRect(window_bounds.size.w / 2.5, window_bounds.size.h / 6, window_bounds.size.w / 10, window_bounds.size.h / 3));
  	init_text_layer(pebble_number_layer, window_layer, FONT_KEY_BITHAM_42_MEDIUM_NUMBERS); 
  	layer_add_child(window_layer, text_layer_get_layer(pebble_number_layer));
	update_pebble_number("");
}
void update_pebble_number(char * message)
{
	text_layer_set_text(pebble_number_layer, message);
}

void hide_pebble_number(bool on_off)
{
	hide_text_layer(pebble_number_layer, on_off);
}