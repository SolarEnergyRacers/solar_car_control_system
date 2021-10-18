//
// Indicators LEFT, RIGTH, WARN
//

#ifndef INDICATOR_H
#define INDICATOR_H

#include <CarState.h>
#include <abstract_task.h>

class Indicator : public abstract_task {
private:
  volatile bool blinkState = false;

public:
  string getName(void);
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void); // this is the actual task
  INDICATOR getIndicator();
  void setIndicator(INDICATOR state);
  void setIndicator(int left, int right);
};

#endif // INDICATOR_H
