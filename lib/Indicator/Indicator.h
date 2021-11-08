//
// Indicators LEFT, RIGTH, WARN
//

#ifndef INDICATOR_H
#define INDICATOR_H

#include <CarState.h>
#include <abstract_task.h>

class Indicator : public abstract_task {
private:
  unsigned long intervall_on = 600;
  unsigned long intervall_off = 300;

public:
  string getName(void);
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void); // this is the actual task
  void setIndicator(INDICATOR state);
  void setIndicator(int left, int right);
  bool getIndicatorLeft();
  bool getIndicatorRight();
};

#endif // INDICATOR_H
