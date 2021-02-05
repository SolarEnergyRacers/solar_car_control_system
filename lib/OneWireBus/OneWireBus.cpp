//
// One Wire Bus
//

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <OneWire.h>

#include "../../include/definitions.h"

#include "OneWireBus.h"

SemaphoreHandle_t onewire_mutex;
OneWire oneWire;

void init_onewire(void){

    // init mutex (it is acquired)
    onewire_mutex = xSemaphoreCreateBinary();

    // init
    OneWire oneWire(ONEWIRE_PIN);

    // release mutex
    xSemaphoreGive(onewire_mutex);
}
