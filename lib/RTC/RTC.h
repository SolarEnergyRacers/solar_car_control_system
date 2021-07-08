//
// Real-Time Clock
//

#ifndef RTC_H
#define RTC_H

#include <abstract_task.h>
#include <RtcDS1307.h>
#include <Wire.h>

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

#endif // RTC_H
