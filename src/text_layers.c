#include "text_layers.h"
#define ACTION_LAYER_WIDTH 20


void init_text_layer(TextLayer* text_layer, Layer* window_layer, const char* font)
{
    text_layer_set_font(text_layer, fonts_get_system_font(font));
	text_layer_set_text(text_layer, "");
	text_layer_set_overflow_mode(text_layer, GTextOverflowModeWordWrap);
	layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

void resize_message(char* message, TextLayer* message_layer)
{
    int message_size = strlen(message);
    if(message_size < 12 )
    {
        text_layer_set_font(message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    }
    else if(message_size < 22)
    {
        text_layer_set_font(message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    }
    else if(message_size < 41 )
    {
        text_layer_set_font(message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    }
    else
    {
        text_layer_set_font(message_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
    }
    text_layer_set_text(message_layer, message);  
}

void hide_text_layer(TextLayer* text_layer, bool on_off)
{
    layer_set_hidden(text_layer_get_layer(text_layer), on_off);  
}