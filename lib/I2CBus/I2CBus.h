//
// I2C Bus
//

#ifndef I2CBUS_H
#define I2CBUS_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

extern SemaphoreHandle_t i2c_mutex;

void init_i2c(void);

#endif // I2CBUS_H
