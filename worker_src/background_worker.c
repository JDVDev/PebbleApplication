#include <pebble_worker.h>

static void init() {
  worker_launch_app();
}

static void deinit() {
  // Deinitialize your worker here

}

int main(void) {
  init();
  worker_event_loop();
  deinit();
}