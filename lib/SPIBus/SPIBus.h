//
// SPI Bus
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_SPIBUS_H
#define SOLAR_CAR_CONTROL_SYSTEM_SPIBUS_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class SPIBus {
private:
public:
  SemaphoreHandle_t mutex = NULL;
  SPIClass spi;
  string init();
  string re_init();
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_SPIBUS_H
