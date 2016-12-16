#include <pebble.h>
#include <string.h>
#include "boolean_task.h"
#include "vibrate.h"
#include "animation.h"
#include "text_layers.h"
#include "clock.h"

Window* window;
MenuLayer *menu_layer;
TextLayer *list_message_layer, *bool_message_layer;
ActionBarLayer *action_bar;
static bool pending_bool_message = false;
static bool pending_list_message = false;
static char *unique_ID;
static int previous_index = 0;
static int menu_y = 0;

#define ACTION_LAYER_WIDTH 20

#define BOOLEAN_MESSAGE 0
#define MENU_MESSAGE 1
#define TEXT_MESSAGE 2

#define MOVE_UP 0
#define MOVE_DOWN 1

#define ANIMATION_DURATION 500

static bool moved_up = false;
static bool moved_down = true;

struct list
{
	char* itemList[20][20];
	int number_of_items;
};

static struct list menuItemList;
  
void exit_app()
{
	window_stack_pop_all(true);
}

void draw_confirmation(char *message)
{
	text_layer_set_text(list_message_layer, message);
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {

  switch(reason) {
    case APP_MSG_SEND_TIMEOUT:     vibrate();
    case APP_MSG_SEND_REJECTED:     vibrate();
    case APP_MSG_NOT_CONNECTED:     vibrate();
    case APP_MSG_APP_NOT_RUNNING:     vibrate();
    case APP_MSG_INVALID_ARGS:     vibrate();
    case APP_MSG_BUSY:     vibrate();
    case APP_MSG_BUFFER_OVERFLOW:     vibrate();
    case APP_MSG_ALREADY_RELEASED:     vibrate();
    case APP_MSG_CALLBACK_ALREADY_REGISTERED:     vibrate();
    case APP_MSG_CALLBACK_NOT_REGISTERED:     vibrate();
    case APP_MSG_OUT_OF_MEMORY:     vibrate();
    case APP_MSG_CLOSED:     vibrate();
    case APP_MSG_INTERNAL_ERROR: vibrate();
    default: break;
  }   
}

// Close boolean menu
void clear_bool_window() 
{
    cancel_vibration_timer();
    layer_remove_from_parent(action_bar_layer_get_layer(action_bar));
    action_bar_layer_clear_icon(action_bar, BUTTON_ID_UP);
    action_bar_layer_clear_icon(action_bar, BUTTON_ID_DOWN);
    action_bar_layer_remove_from_window(action_bar);
    pending_bool_message = false;
//    hide_text_layer(list_message_layer, true);
    hide_clock(false);
}

// Close list menu
void clear_list_window() 
{
    cancel_vibration_timer();
    pending_list_message = false;
    layer_remove_from_parent(menu_layer_get_layer(menu_layer));
    hide_clock(false);
}

// Send message to Task Relay
static void send(int key, int value)
{
	DictionaryIterator *iter;

	app_message_outbox_begin(&iter);
	dict_write_int(iter, key, &value, sizeof(int), true);
	dict_write_cstring(iter, 3, unique_ID);
	dict_write_end(iter);
	app_message_outbox_send();
}

// Get size of list menu
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
	return menuItemList.number_of_items;
}

void display_text_message(char* message)
{
  	//clear_bool_window();
    //reset_text_layer_position(bool_message_layer);
    //	Layer *window_layer = window_get_root_layer(window);
    //	GRect window_bounds = layer_get_bounds(window_layer);
	// Create output TextLayer
  	 //layer_set_frame(text_layer_get_layer(bool_message_layer), GRect(0, -2, window_bounds.size.w - (2*ACTION_LAYER_WIDTH), window_bounds.size.h ));
      text_layer_set_text(bool_message_layer, message);
}

// Read received list
void read_list(char* list)
{
	int string_number = 0;
	char *listItem = strtok(list, ";");

	while (listItem != NULL)
	{
		menuItemList.itemList[string_number][0] = listItem;
		listItem = strtok(NULL, ";");
		++string_number;
	}
	menuItemList.number_of_items = string_number;
}

void redraw_message()
{
    if(moved_down == true)
        layer_set_hidden(text_layer_get_layer(list_message_layer), false);
}

void check_menu_index()
{
    int highlighted_index = menu_layer_get_selected_index(menu_layer).row;
    
    if(highlighted_index < previous_index)
    {
        moved_down = true;
        moved_up = false;
        move_menu(MOVE_DOWN, menu_layer, highlighted_index);
        move_text(MOVE_DOWN, list_message_layer, highlighted_index);
    }
    
    else if(highlighted_index > previous_index)
    {
        moved_up = true;
        moved_down = false;
        move_menu(MOVE_UP, menu_layer,highlighted_index);
        move_text(MOVE_UP, list_message_layer, highlighted_index);
    }
    previous_index = highlighted_index;
}

void draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
	int row = cell_index->row;
    check_menu_index();
	for (int i = 0; i < menuItemList.number_of_items; i++)
	{
		if (row == i) { menu_cell_basic_draw(ctx, cell_layer, menuItemList.itemList[i][0], "", NULL); break; }
	}
}

void reset_menu_index()
{
	MenuIndex index;
	index.row = 0;
	menu_layer_set_selected_index(menu_layer, index, MenuRowAlignCenter, false);
}

void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{
    if(pending_list_message == true)
    {
        int selectedItem = cell_index->row;
        send(MENU_MESSAGE, selectedItem);
        layer_set_hidden(text_layer_get_layer(list_message_layer), false);
        text_layer_set_text(list_message_layer, menuItemList.itemList[selectedItem][0]);
        clear_list_window();
    }
}

// Bind "Accept" to up click
void up_click_handler(ClickRecognizerRef recognizer, void *context)
{
	text_layer_set_text(bool_message_layer, "Accepted");
	send(BOOLEAN_MESSAGE, 0);
	clear_bool_window();   
}

// Bind "Decline" to down click
void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
	text_layer_set_text(bool_message_layer, "Declined!");
	send(BOOLEAN_MESSAGE, 1);
	clear_bool_window();
}

//override back button to prevent app from being exited
void back_click_handler(ClickRecognizerRef recognizer, void *context){}

// Register the ClickHandler
void click_config_provider(void *context)
{
	window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void back_click_config_provider(void * context)
{
    window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

void register_menu_layer_callbacks()
{
	static MenuLayerCallbacks callbacks =
	{
		.draw_row = (MenuLayerDrawRowCallback)draw_row_callback,
		.get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)num_rows_callback,
		.select_click = (MenuLayerSelectCallback)select_click_callback,
    
	};
	menu_layer_set_callbacks(menu_layer, NULL, callbacks);
}

void create_menu()
{
	menu_layer = menu_layer_create(GRect(0, 100, 144, 126));
    //Let it receive clicks
	menu_layer_set_click_config_onto_window(menu_layer, window);
  
	//Give it its callbacks
	register_menu_layer_callbacks();
}

void create_text_layers(Window *window)
{
	Layer *window_layer = window_get_root_layer(window);
	GRect window_bounds = layer_get_bounds(window_layer);
	// Create output TextLayer
	list_message_layer = text_layer_create(GRect(0, -2, window_bounds.size.w, window_bounds.size.h));
    bool_message_layer = text_layer_create(GRect(0, -2, window_bounds.size.w - (2*ACTION_LAYER_WIDTH), window_bounds.size.h ));
    init_text_layer(list_message_layer, window_layer, FONT_KEY_GOTHIC_14_BOLD);
    init_text_layer(bool_message_layer, window_layer, FONT_KEY_GOTHIC_28_BOLD);
}

void create_clock()
{
    draw_clock(window);
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);  
}

void create_action_layer()
{
	// Initialize the action bar:
	action_bar = action_bar_layer_create();
	// Associate the action bar with the window:
	action_bar_layer_add_to_window(action_bar, window);
	// Set the click config provider:
	action_bar_layer_set_click_config_provider(action_bar,
		click_config_provider);
}

void open_menu(char *message)
{
    reset_menu_attributes();
    hide_clock(true);
    layer_set_hidden(text_layer_get_layer(bool_message_layer), true);
    layer_set_hidden(text_layer_get_layer(list_message_layer), false);
    read_list(message);
    menu_layer_reload_data(menu_layer);
    layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
    reset_menu_index();
}

void clear_window()
{
    if(pending_bool_message == true)
    {
        clear_bool_window();
    }
    else if (pending_list_message == true)
    {
        clear_list_window();    
    }
}

void init_bool_menu(char *message)
{
    hide_clock(true);
    layer_set_hidden(text_layer_get_layer(bool_message_layer), false);
    create_action_layer();
    draw_boolean_menu(message, bool_message_layer, action_bar);
    pending_bool_message = true;
}

void init_list_menu(char *message, char *menu_items)
{
    menu_layer_set_click_config_onto_window(menu_layer, window);
    open_menu(menu_items);
    text_layer_set_text(list_message_layer, message);  
    move_text(1, list_message_layer, 0);
    pending_list_message = true;
}

// Handle received messages
static void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
  clear_window();

  Tuple *t = dict_read_first(iterator);
	if (t != NULL)
	{
		int message_type = t->key;
		char* received_message = t->value->cstring;
        
		t = dict_read_next(iterator);
		unique_ID = t->value->cstring;
        if (message_type == BOOLEAN_MESSAGE)
        {
            if(pending_bool_message){clear_bool_window();}            
            init_bool_menu(received_message);
            register_vibration_timer();
        }
        else if (message_type == MENU_MESSAGE)
        {
            resize_message(received_message, list_message_layer);
            t = dict_read_next(iterator);
            char* messageList = t->value->cstring;
            init_list_menu(received_message, messageList);
            register_vibration_timer();
        }
        else if (message_type == TEXT_MESSAGE)
        {
            display_text_message(received_message);
        }
	} 
}

void window_load(Window *window)
{
    create_menu();
    create_text_layers(window);
    create_clock();
    //draw_pebble_number(window);
    window_set_click_config_provider(window, back_click_config_provider);
}

void window_unload(Window *window)
{
	menu_layer_destroy(menu_layer);
	text_layer_destroy(list_message_layer);
    text_layer_destroy(bool_message_layer);
	action_bar_layer_destroy(action_bar);
}

void init()
{
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload
	});
	window_stack_push(window, true);
	app_message_register_inbox_received(inbox_received_callback);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    window_set_fullscreen(window, true);
    app_message_register_outbox_failed(outbox_failed_callback);
}

void deinit()
{
	app_message_deregister_callbacks();
	window_destroy(window);
}

int main(void)
{
	init();
	app_event_loop();
	deinit();
}