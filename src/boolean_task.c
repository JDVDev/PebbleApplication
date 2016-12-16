#include <pebble.h>
#include "boolean_task.h"

void draw_boolean_menu(char *message, TextLayer *output_layer, ActionBarLayer *action_bar)
{
	text_layer_set_text(output_layer, message);
	//	store_current_task(s_buffer);
	
	GBitmap *yes_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_YES);
	GBitmap *no_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_NO);

	action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, yes_icon);
	action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, no_icon);
}