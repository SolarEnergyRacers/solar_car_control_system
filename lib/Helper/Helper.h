//
// Helper Functions
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_HELPER_H
#define SOLAR_CAR_CONTROL_SYSTEM_HELPER_H

char *fgets_stdio_blocking(char *str, int n);
void xSemaphoreTakeT(QueueHandle_t mutex);

#endif // SOLAR_CAR_CONTROL_SYSTEM_HELPER_H
