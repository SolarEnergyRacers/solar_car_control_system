//
// One Wire Bus
//

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <OneWire.h>

#include <definitions.h>

#include <OneWireBus.h>

void OneWireBus::init(void) {

  // init mutex (it is acquired)
  mutex = xSemaphoreCreateBinary();

  // init
  oneWire = OneWire(ONEWIRE_PIN);

  // release mutex
  xSemaphoreGive(mutex);

  printf("[v] OneWire inited: ONEWIRE_PIN=%d.\n", ONEWIRE_PIN);
}
