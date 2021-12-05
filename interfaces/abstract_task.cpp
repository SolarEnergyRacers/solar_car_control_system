//
// This is an example class
//
#include <iostream>
#include <stdlib.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <abstract_task.h>

using namespace std;

void abstract_task::init() {
  // do initialization in implementation here
  cout << "[?] Init '" << getInfo() << "'... ";
};

void abstract_task::sleep() { vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS); };
void abstract_task::sleep(int polling_ms) { vTaskDelay(polling_ms / portTICK_PERIOD_MS); };

void abstract_task::create_task(int priority) {
  cout << " - create task '" << getInfo() << "'...";
#if WithTaskSuspend == true
  xTaskCreate((void (*)(void *)) & init_task, getInfo().c_str(), 4096, (void *)this, 1, &xHandle);
#else
  xTaskCreate((void (*)(void *)) & init_task, getInfo().c_str(), 4096, (void *)this, priority, NULL);
#endif
  cout << " done." << endl;
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

void abstract_task::set_SleepTime(int milliseconds) { sleep_polling_ms = milliseconds; }
