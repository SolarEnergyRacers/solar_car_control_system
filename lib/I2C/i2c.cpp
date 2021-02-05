//
// Created by andre on 2/5/2021.
//

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <Wire.h> // Arduino I2C library

#include "../../include/definitions.h"

#include "i2c.h"

SemaphoreHandle_t i2c_mutex;

void init_i2c(void) {
    // init mutex
    i2c_mutex = xSemaphoreCreateBinary();
    // release mutex
    xSemaphoreGive(i2c_mutex);

    // init i2c wire library
    Wire.begin(I2C_SDA, I2C_SCL, I2C_FREQ);

}