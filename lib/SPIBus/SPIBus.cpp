//
// SPI Bus
//

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <SPI.h>

#include "../../include/definitions.h"

#include "SPIBus.h"

SemaphoreHandle_t spi_mutex;

void init_spi(void){

    // init mutex (it is acquired)
    spi_mutex = xSemaphoreCreateBinary();

    // release mutex
    xSemaphoreGive(spi_mutex);
}

