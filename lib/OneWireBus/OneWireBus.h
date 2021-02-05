//
// One Wire Bus
//

#ifndef ONEWIREBUS_H
#define ONEWIREBUS_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <OneWire.h> // 1-Wire

extern SemaphoreHandle_t onewire_mutex;
extern OneWire oneWire;

void init_onewire(void);

#endif // ONEWIREBUS_H
