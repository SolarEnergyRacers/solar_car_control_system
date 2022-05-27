//
// Indicators LEFT, RIGTH, WARN
//

#ifndef INDICATOR_H
#define INDICATOR_H

#include <CarState.h>
#include <abstract_task.h>

class Indicator : public abstract_task {
private:
  uint32_t sleep_polling_ms = 400;

public:
  // RTOS task
  void set_SleepTime(uint32_t milliseconds) { sleep_polling_ms = milliseconds; };
  uint32_t get_SleepTime() { return sleep_polling_ms; };
  string getName(void) { return "Indicator"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);

  // Class functions and members
  void setIndicator(INDICATOR state);
  void setIndicator(int left, int right);
};

#endif // INDICATOR_H
