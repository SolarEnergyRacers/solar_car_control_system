//
// Real-Time Clock
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_RTC_H
#define SOLAR_CAR_CONTROL_SYSTEM_RTC_H

#include <RtcDS1307.h>
#include <Wire.h>
#include <abstract_task.h>

class RTC : public abstract_task {
private:
  uint32_t sleep_polling_ms = 400;

public:
  // RTOS task
  void set_SleepTime(uint32_t milliseconds) { sleep_polling_ms = milliseconds; };
  uint32_t get_SleepTime() { return sleep_polling_ms; };
  string getName(void) { return "RTC"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);

  // Class functions and members
private:
  RtcDS1307<TwoWire> Rtc = RtcDS1307<TwoWire>(Wire);

public:
  RtcDateTime read_rtc_datetime(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_RTC_H
