//
// One Wire Bus
//

#ifndef ONEWIREBUS_H
#define ONEWIREBUS_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <OneWire.h> // 1-Wire

class OneWireBus {
private:
public:
  SemaphoreHandle_t mutex;
  OneWire oneWire;
  void init();
  void re_init();
};

#endif // ONEWIREBUS_H
