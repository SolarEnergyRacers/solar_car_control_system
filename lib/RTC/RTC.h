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
  RtcDS1307<TwoWire> Rtc = RtcDS1307<TwoWire>(Wire);

public:
  string getName(void) { return "RTC"; };
  void init(void);
  void re_init(void);
  void exit(void);
  void task(void);
  RtcDateTime read_rtc_datetime(void);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_RTC_H
