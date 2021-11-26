//
// This is an example class
//

#ifndef ABSTRACT_TASK_H
#define ABSTRACT_TASK_H

#include <definitions.h>
#include <stdint.h>
#include <string>

#if WithTaskSuspend == true
#include <task.h>
// typedef void *TaskHandle_t;
#endif

using namespace std;

class abstract_task {
private:
  // put private/internal variables/functions here
#if WithTaskSuspend == true
  TaskHandle_t xHandle;
#endif

protected:
  uint32_t sleep_polling_ms = 1111;

public:
  virtual string getName(void);
  virtual void init(void);
  virtual void re_init(void);
  virtual void exit(void);
  virtual void task(void); // this is the actual task

  string getInfo(void);
  void set_SleepTime(int milliseconds);

#if WithTaskSuspend == true
  TaskHandle_t getHandle() { return xHandle; };
#endif

  void sleep(void);
  void sleep(int polling_ms);

  void create_task(int priority = 1);
  static void init_task(void *pvParams) { ((abstract_task *)pvParams)->task(); };
};

#endif // ABSTRACT_TASK_H
