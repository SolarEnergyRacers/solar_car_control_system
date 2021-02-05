//
// One Wire Bus
//

#ifndef ONEWIREBUS_H
#define ONEWIREBUS_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

extern SemaphoreHandle_t onewire_mutex;

void init_onewire(void);

#endif // ONEWIREBUS_H
