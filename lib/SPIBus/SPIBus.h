//
// SPI Bus
//

#ifndef SPIBUS_H
#define SPIBUS_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class SPIBus {
private:
public:
  SemaphoreHandle_t mutex;
  void init();
  void re_init();
};

#endif // SPIBUS_H
