#include <pebble.h>
#include "clock.h"
#include "text_layers.h"

TextLayer *clock_layer;

void draw_clock(Window* window)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect window_bounds = layer_get_bounds(window_layer);
    // Create output TextLayer
    clock_layer = text_layer_create(GRect(window_bounds.size.w / 6, window_bounds.size.h /2, window_bounds.size.w, window_bounds.size.h));
    init_text_layer(clock_layer, window_layer, FONT_KEY_BITHAM_42_MEDIUM_NUMBERS); 
    layer_add_child(window_layer, text_layer_get_layer(clock_layer));
}

void hide_clock(bool on_off)
{
    hide_text_layer(clock_layer, on_off);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed) 
{
    update_time();
} 

void update_time() {
    // Get a tm structure
    time_t temp = time(NULL); 
    struct tm *tick_time = localtime(&temp);

    // Write the current hours and minutes into a buffer
    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                            "%H:%M" : "%I:%M", tick_time);

    // Display this time on the TextLayer
    text_layer_set_text(clock_layer, s_buffer);
}
