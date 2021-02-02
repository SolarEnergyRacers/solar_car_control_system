/*
 *
 */
#ifndef DEFINITIONS_H
#define DEFINITIONS_H


#define LED_BUILTIN (gpio_num_t)22

#define I2C_SDA 33
#define I2C_SCL 32
#define I2C_FREQ 100000
SemaphoreHandle_t i2c_mutex;


#endif // DEFINITIONS_H
