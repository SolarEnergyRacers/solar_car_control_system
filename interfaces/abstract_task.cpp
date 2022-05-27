//
// This is the abstract task class, which has to be implemented by all other tasks
//
#include <iostream>
#include <stdint.h>
#include <stdlib.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <../lib/Console/Console.h>
#include <abstract_task.h>

using namespace std;

extern Console console;

void abstract_task::init() {
  // do initialization in implementation here
  console << "[?] Init '" << getInfo() << "'... ";
};

void abstract_task::sleep() { vTaskDelay(get_SleepTime() / portTICK_PERIOD_MS); };

void abstract_task::create_task(int priority, uint32_t sleep_polling, int stack_size) {
  // sleep_polling_ms = sleep_polling;
  set_SleepTime(sleep_polling);
  console << " - create task '" << getInfo() << "'...";
#if WithTaskSuspend == true
  xTaskCreate((void (*)(void *)) & init_task, getInfo().c_str(), stack_size, (void *)this, 1, &xHandle);
#else
  xTaskCreate((void (*)(void *)) & init_task, getInfo().c_str(), stack_size, (void *)this, priority, NULL);
#endif
  console << " done.\n";
};

void abstract_task::re_init() {
#if WithTaskSuspend == true
  vTaskResume(xHandle);
#endif
};

void abstract_task::exit() {
#if WithTaskSuspend == true
  vTaskDelete(xHandle);
#endif
};

string abstract_task::getInfo() { return getName(); }
