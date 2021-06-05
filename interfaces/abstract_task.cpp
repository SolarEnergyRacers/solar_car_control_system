//
// This is an example class
//
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdlib.h>

#include "abstract_task.h"

using namespace std;

void abstract_task::init(){
    // do initialization here
};

void abstract_task::create_task() {

  void run(void *pvParams) { ((MyClass *)pcParams)->runInner(); }
  xTaskCreate((void (*)(void *))task<MyClass>, taskName, stackDepth, x,
              taskPrio, taskHandle);
  // xTaskCreate(&task, getName().c_str(),
  // CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
};

void abstract_task::sleep() {
  vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
};

static void abstract_task::task(void *pvParameter) {
  ((MyClass *)pcParams)->task();
};

void abstract_task::re_init(){
    // handle reset here
};

void abstract_task::exit(){
    // handle exit here
};

string abstract_task::getInfo() { return getName(); }
