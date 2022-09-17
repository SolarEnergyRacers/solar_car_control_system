//
// Real-Time Clock
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_RTC_H
#define SOLAR_CAR_CONTROL_SYSTEM_RTC_H

#include <../.pio/libdeps/esp32dev/RTC/src/RtcDS1307.h>
#include <Wire.h>
#include <abstract_task.h>

class RTC : public abstract_task {
public:
  // RTOS task
  string getName(void) { return "RTC"; };
  string init(void);
  string re_init(void);
  void exit(void);
  void task(void);

  // Class functions and members
private:
  RtcDS1307<TwoWire> Rtc = RtcDS1307<TwoWire>(Wire);

public:
  RtcDateTime read_rtc_datetime(void);
  void write_rtc_datetime(RtcDateTime dateTime);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_RTC_H
