//
// I2C Bus
//

#ifndef I2CBUS_H
#define I2CBUS_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

extern SemaphoreHandle_t i2c_mutex;

void init_i2c(void);
bool i2c_available(uint8_t adr);
void scan_i2c_devices();

#endif // I2CBUS_H
