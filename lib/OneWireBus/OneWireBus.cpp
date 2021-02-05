//
// One Wire Bus
//

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <OneWire.h>

#include "OneWireBus.h"

SemaphoreHandle_t onewire_mutex;

void init_onewire(void){

    // init mutex (it is acquired)
    onewire_mutex = xSemaphoreCreateBinary();

    // release mutex
    xSemaphoreGive(onewire_mutex);
}
