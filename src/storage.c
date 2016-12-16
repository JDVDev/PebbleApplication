#include "storage.h"
#include <pebble.h>
#define PEBBLE_NUMBER_KEY 0

void write_pebble_key_to_storage(int pebble_number)
{
	persist_write_int(PEBBLE_NUMBER_KEY, pebble_number);	
}

int read_pebble_key_from_storage()
{
	return persist_read_int(PEBBLE_NUMBER_KEY);
}