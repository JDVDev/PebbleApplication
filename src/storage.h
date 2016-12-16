#ifndef STORAGE_H
#define STORAGE_H

#include <pebble.h>

void write_pebble_key_to_storage(int pebble_number);
int read_pebble_key_from_storage();

#endif