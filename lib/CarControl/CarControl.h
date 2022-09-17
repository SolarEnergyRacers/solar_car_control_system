/*
 * Car controll loops
 */

#ifndef SER_CAR_CONTROL_H
#define SER_CAR_CONTROL_H

#include <list>
#include <map>
#include <string>

//#include <MCP23017.h>

#include <abstract_task.h>
#include <definitions.h>

const int MAX_ACCELERATION_DISPLAY_VALUE = 99; // absolute of minimal or maximal value for acceleration scaling

class CarControl : public abstract_task {

public:
  // RTOS task
  string getName(void) { return "CarControl"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  // Class member and functions
  static void valueChangedHandler() { valueChangeRequest++; };

  bool read_battery_data();
  bool read_motor_data();
  bool read_pv_data();
  bool read_reference_cell_data();
  bool read_speed(); // in km/h

  int calculate_displayvalue_acc_dec(int valueDec, int valueAcc);
  bool read_paddles();
  bool read_PLUS_MINUS();
  void adjust_paddles(int second = 1);
  void reset_acceleration_values() { _set_dec_acc_values(0, 0, 0, 0, 0); }
  bool verboseMode = false;

private:
  int ads_min_dec = 14000; // paddle_adjust overwrites this values
  int ads_min_acc = 21000;
  int ads_max_dec = 22000;
  int ads_max_acc = 23000;
  int accelLast = 0;
  int recupLast = 0;
  int valueDisplayLast = 0;
  bool justInited = true;

  void _set_dec_acc_values(int valueDecPot, int valueAccPot, int16_t valueDec, int16_t valueAcc, int valueDisplay);
  void _handle_indicator();

  static volatile int valueChangeRequest;
  bool isInValueChangedHandler = false;
  void _handleValueChanged();

  int _normalize(int minDisplayValue, int maxDisplayValue, int minValue, int maxValue, int value);
};
#endif // SER_CAR_CONTROL_H
