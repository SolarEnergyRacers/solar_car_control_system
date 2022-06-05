//
// One Wire Bus
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_ONEWIREBUS_H
#define SOLAR_CAR_CONTROL_SYSTEM_ONEWIREBUS_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <OneWire.h> // 1-Wire

class OneWireBus {
private:
public:
  SemaphoreHandle_t mutex = NULL;
  OneWire oneWire;
  string init();
  string re_init();
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_ONEWIREBUS_H
