//
// This is an example class
//
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdlib.h>

#include "abstract_task.h"

using namespace std;

void abstract_task::init() {
  // do initialization here
  printf(" - init '%s'... ", getInfo().c_str());
};

void abstract_task::sleep() {
  vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
};

void abstract_task::create_task() {
  printf(" - create task '%s'... ", getInfo().c_str());
  xTaskCreate((void (*)(void *)) & init_task, getInfo().c_str(), 1024,
              (void *)this, 1, NULL);
};

void abstract_task::re_init(){
    // handle reset here
};

void abstract_task::exit(){
    // handle exit here
};

string abstract_task::getInfo() { return getName(); }
