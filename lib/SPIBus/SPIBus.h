//
// SPI Bus
//

#ifndef SPIBUS_H
#define SPIBUS_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

extern SemaphoreHandle_t spi_mutex;

void init_spi(void);

#endif // SPIBUS_H
