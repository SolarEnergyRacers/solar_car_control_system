//
// Helper Functions
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_HELPER_H
#define SOLAR_CAR_CONTROL_SYSTEM_HELPER_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore
#include <string>

#include <RTC.h>

using namespace std;

char *fgets_stdio_blocking(char *str, int n);
void xSemaphoreTakeT(xQueueHandle mutex);
string formatDateTime(RtcDateTime now);
string getTimeStamp(unsigned long seconds);

#endif // SOLAR_CAR_CONTROL_SYSTEM_HELPER_H
