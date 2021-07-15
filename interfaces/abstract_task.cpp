//
// This is an example class
//
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdlib.h>

#include "abstract_task.h"

using namespace std;

void abstract_task::init() {
  // do initialization in implementation here
  printf(" - init '%s'... ", getInfo().c_str());
};

void abstract_task::sleep() { vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS); };

void abstract_task::create_task() {
  printf(" - create task '%s'...", getInfo().c_str());
  xTaskCreate((void (*)(void *)) & init_task, getInfo().c_str(), 4096, (void *)this, 1, NULL);
  printf(" done.\n");
};

void abstract_task::re_init(){
    // handle reset in implementation here
};

void abstract_task::exit(){
    // handle exit in implementation here
};

string abstract_task::getInfo() { return getName(); }
