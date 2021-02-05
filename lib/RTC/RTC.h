//
// Real-Time Clock
//

#ifndef RTC_H
#define RTC_H

#include <RtcDS1307.h>

void init_rtc(void);
RtcDateTime read_rtc_datetime(void);
void read_rtc_demo_task(void *pvParameter);

#endif // RTC_H
