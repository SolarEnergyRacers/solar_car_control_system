//
// This is an example class
//

#ifndef ABSTRACT_TASK_H
#define ABSTRACT_TASK_H

#include <stdint.h>
#include <string>

using namespace std;

class abstract_task {
private:
  // put private/internal variables/functions here
  uint32_t sleep_polling_ms = 1000;

public:
  virtual string getName(void);
  virtual void init(void);
  virtual void re_init(void);
  virtual void exit(void);
  virtual void runInner();

  string getInfo(void);

  void sleep(void);
  void create_task(void);
  static void task(void *pvParameter);
};

#endif // ABSTRACT_TASK_H
