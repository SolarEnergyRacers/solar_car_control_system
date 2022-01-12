//
// Helper Functions
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_HELPER_H
#define SOLAR_CAR_CONTROL_SYSTEM_HELPER_H

#include <FreeRTOS.h>
#include <string>

// using namespace std;

char *fgets_stdio_blocking(char *str, int n);
void xSemaphoreTakeT(xQueueHandle mutex);
std::string getTimeStamp(unsigned long seconds);

#endif // SOLAR_CAR_CONTROL_SYSTEM_HELPER_H
