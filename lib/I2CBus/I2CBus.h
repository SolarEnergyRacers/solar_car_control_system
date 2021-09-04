//
// I2C Bus
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_I2CBUS_H
#define SOLAR_CAR_CONTROL_SYSTEM_I2CBUS_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class I2CBus {
private:
public:
  SemaphoreHandle_t mutex; // TODO: maybe we want to hide the mutex behind a lock() / unlock() function?
  void init();
  void re_init();
  bool i2c_available(uint8_t adr);
  void scan_i2c_devices();
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_I2CBUS_H
