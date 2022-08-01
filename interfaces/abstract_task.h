//
// This is the abstract task class, which has to be implemented by all other tasks
//

#ifndef ABSTRACT_TASK_H
#define ABSTRACT_TASK_H

#include <definitions.h>
#include <stdint.h>
#include <string>

#if WithTaskSuspend == true
#include <task.h>
#endif

using namespace std;

class abstract_task {
private:
  // put private/internal variables/functions here
#if WithTaskSuspend == true
  TaskHandle_t xHandle;
#endif

public:
  virtual string getName(void);
  virtual string init(void) = 0;
  virtual string re_init(void) = 0;
  virtual void exit(void);
  virtual void task(void) = 0;

  string getInfo(void);

#if WithTaskSuspend == true
  TaskHandle_t getHandle() { return xHandle; };
#endif

  uint32_t sleep_polling_ms;
  void sleep(void);
  void sleep(int polling_ms);
  string report_task_init();
  string report_task_init(abstract_task *task);
  string create_task(int priority = 10, uint32_t sleep_polling = 330, int stack_size = 4096);
  static void init_task(void *pvParams) { ((abstract_task *)pvParams)->task(); };
};

#endif // ABSTRACT_TASK_H
