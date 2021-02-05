//
// I2C Bus
//

#ifndef I2C_H
#define I2C_H

extern SemaphoreHandle_t i2c_mutex;

void init_i2c(void);

#endif // I2C_H
