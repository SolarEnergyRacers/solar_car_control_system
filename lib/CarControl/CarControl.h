/*
 * PCF8574 I/O Extension over I2C  !!! UNTESTED !!!
 */

#ifndef SER_CAR_CONTROL_H
#define SER_CAR_CONTROL_H

#include <list>
#include <map>
#include <string>

#include <PCF8574.h>

#include <abstract_task.h>
#include <definitions.h>

class CarControl {
public:
  static void valueChangedHandler() { valueChangeRequest++; };

  string getName(void) { return "IOExt"; };

  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);

private:
  int taskSleep = 50;

  static volatile int valueChangeRequest;
  bool isInValueChangedHandler = false;
  void handleValueChanged();
};
#endif // SER_CAR_CONTROL_H IOEXT_H
