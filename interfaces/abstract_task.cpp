//
// This is an example class
//
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdlib.h>

#include <abstract_task.h>

using namespace std;

void abstract_task::init() {
  // do initialization in implementation here
  printf("[?] Init '%s'... ", getInfo().c_str());
};

void abstract_task::sleep() { vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS); };
void abstract_task::sleep(int polling_ms) { vTaskDelay(polling_ms / portTICK_PERIOD_MS); };

void abstract_task::create_task(int priority) {
  printf(" - create task '%s'...", getInfo().c_str());
#if WithTaskSuspend == true
  xTaskCreate((void (*)(void *)) & init_task, getInfo().c_str(), 4096, (void *)this, 1, &xHandle);
#else
  xTaskCreate((void (*)(void *)) & init_task, getInfo().c_str(), 4096, (void *)this, priority, NULL);
#endif
  printf(" done.\n");
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
