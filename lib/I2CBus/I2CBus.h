//
// I2C Bus
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_I2CBUS_H
#define SOLAR_CAR_CONTROL_SYSTEM_I2CBUS_H

#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

using namespace std;

class I2CBus {
private:
public:
  SemaphoreHandle_t mutex = NULL;
  void init();
  void re_init();
  bool i2c_available(uint8_t adr);
  void scan_i2c_devices();
  string getName() { return "I2C Bus"; }
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_I2CBUS_H
