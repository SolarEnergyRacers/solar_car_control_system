/*
 * Car controll loops
 */

#ifndef SER_CAR_CONTROL_H
#define SER_CAR_CONTROL_H

#include <list>
#include <map>
#include <string>

#include <MCP23017.h>

#include <abstract_task.h>
#include <definitions.h>

class CarControl : public abstract_task {
private:
  // uint32_t sleep_polling_ms = 400;

public:
  // RTOS task
  // void set_SleepTime(uint32_t milliseconds) { sleep_polling_ms = milliseconds; };
  // uint32_t get_SleepTime() { return sleep_polling_ms; };
  string getName(void) { return "CarControl"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  // Class member and functions
  SemaphoreHandle_t mutex = NULL;
  static void valueChangedHandler() { valueChangeRequest++; };

  bool read_battery_data();
  bool read_motor_data();
  bool read_pv_data();
  bool read_reference_cell_data();
  bool read_speed();

  bool read_paddles();
  void adjust_paddles(int second = 1);

private:
  const int MIN_ADJUST_GAP = 10; // 0...Gap units show 0 (real reachable null point)
  const int MAX_ADJUST_GAP = 10; // max ... max+Gap to avoid max higher then adjusted max
  const int SMOOTHING = 1;
  const int MIN_DISPLAY_VALUE = 99; // absolute of minimal value for negative scaling
  const int MAX_DISPLAY_VALUE = 99;

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
