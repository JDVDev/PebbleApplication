#include "animation.h"

#define ANIMATION_DURATION 500
#define MAX_Y 100
#define MAX_TEXT_Y -2
#define OFFSET 20
int menu_y = MAX_Y;
int text_y = 0;

void reset_menu_attributes()
{
  //menu_y = MAX_Y;
  //text_y = 0;
}


void move_menu(int direction, MenuLayer* menu_layer, int index) 
{
    // Determine start and finish positions
    GRect start, finish;
    switch (direction) {
        case 0:
        start = GRect(0, menu_y, 144, 160);
        if(menu_y > 0 && index < 6) menu_y-= OFFSET;
        finish = GRect(0, menu_y, 144, 160);
        break;
        case 1:
        start = GRect(0, menu_y, 144, 160);
        if(menu_y< MAX_Y && index < 6) menu_y+= OFFSET;
        finish = GRect(0, menu_y, 144, 160);
        break;
    }

    // Schedule the next animation
    menu_animation = property_animation_create_layer_frame(menu_layer_get_layer(menu_layer), &start, &finish);
    animation_set_duration((Animation*)menu_animation, ANIMATION_DURATION);
    animation_set_curve((Animation*)menu_animation, AnimationCurveEaseInOut);
    animation_schedule((Animation*)menu_animation);  
}

void move_text(int direction, TextLayer* text_layer, int index)
{
    // Determine start and finish positions
    GRect start, finish;
    switch (direction) {
        case 0:
        start = GRect(0, text_y, 144, 160);
        if( text_y > -100 && index < 6 ) text_y-= OFFSET;
        finish = GRect(0, text_y, 144, 160);
        break;
        case 1:
        start = GRect(0, text_y, 144, 160);
        if( text_y < MAX_TEXT_Y && index < 6) text_y+= OFFSET;
        finish = GRect(0, text_y, 144, 160);
        break;
    }
    // Schedule the next animation
    menu_animation = property_animation_create_layer_frame(text_layer_get_layer(text_layer), &start, &finish);
    animation_set_duration((Animation*)menu_animation, ANIMATION_DURATION);
    animation_set_curve((Animation*)menu_animation, AnimationCurveEaseInOut);
    animation_schedule((Animation*)menu_animation);  
}

void reset_text_layer_position(TextLayer* text_layer)
{
    layer_set_frame(text_layer_get_layer(text_layer), GRect(0, 0, 144, 153));
    /*
    GRect start, finish;
    finish = GRect(0,0, 144, 160);
    start = GRect(0,0, 144, 160);
    menu_animation = property_animation_create_layer_frame(text_layer_get_layer(text_layer), &start, &finish);
    animation_set_duration((Animation*)menu_animation, ANIMATION_DURATION);
    animation_set_curve((Animation*)menu_animation, AnimationCurveEaseInOut);
    animation_schedule((Animation*)menu_animation);  */
}