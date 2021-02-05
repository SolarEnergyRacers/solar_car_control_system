//
// I2C Bus
//

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <Wire.h> // Arduino I2C library

#include "../../include/definitions.h"

#include "i2c.h"

SemaphoreHandle_t i2c_mutex;

void init_i2c(void) {

    // CRITICAL SECTION I2C: start
    // init mutex (it is acquired)
    i2c_mutex = xSemaphoreCreateBinary();

    // init i2c wire library
    Wire.begin(I2C_SDA, I2C_SCL, I2C_FREQ);

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end
}
