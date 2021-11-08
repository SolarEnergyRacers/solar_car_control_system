/*
 * Car controll loops
 */

#ifndef SER_CAR_CONTROL_H
#define SER_CAR_CONTROL_H

#include <list>
#include <map>
#include <string>

#include <MCP23017.h>
#include <PCF8574.h>

#include <abstract_task.h>
#include <definitions.h>

class CarControl : public abstract_task {
public:
  SemaphoreHandle_t mutex = NULL;
  static void valueChangedHandler() { valueChangeRequest++; };

  string getName(void) { return "CarControl"; };

  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);

  bool read_battery_data();
  bool read_motor_data();
  bool read_pv_data();
  bool read_speed();

  bool read_paddles();
  void adjust_paddles_min();
  void adjust_paddles_max();

private:
  const int MIN_ADJUST_GAP = 10; // 0...Gap units show 0 (real reachable null point)
  const int MAX_ADJUST_GAP = 10; // max ... max+Gap to avoid max higher then adjusted max
  const int SMOOTHING = 1;
  const int MAX_DISPLAY_VALUE = 99;

  int16_t ads_min_dec = 0;
  int16_t ads_min_acc = 0;
  int16_t ads_max_dec = 25000;
  int16_t ads_max_acc = 25000;
  int16_t accelLast = 0;
  int16_t recupLast = 0;
  int16_t valueDisplayLast = 0;
  bool justInited = true;

  void _handle_indicator();

  static volatile int valueChangeRequest;
  bool isInValueChangedHandler = false;
  void _handleValueChanged();

  int _normalize(int value, int maxValue, int min, int max);
};
#endif // SER_CAR_CONTROL_H
