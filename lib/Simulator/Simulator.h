//
// Simulator
//

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

void init_simulator(void);
void simulator_task(void *pvParameter);

#endif // SIMULATOR_H