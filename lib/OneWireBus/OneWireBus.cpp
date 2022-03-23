//
// One Wire Bus
//
#include <definitions.h>

#include <fmt/core.h>
#include <inttypes.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h> // semaphore

#include <Console.h>
#include <OneWire.h>
#include <OneWireBus.h>

extern Console console;
using namespace std;

void OneWireBus::init(void) {

  // init mutex (it is acquired)
  mutex = xSemaphoreCreateMutex();

  // init
  oneWire = OneWire(ONEWIRE_PIN);

  // release mutex
  xSemaphoreGive(mutex);

  console << "[v] OneWire inited: ONEWIRE_PIN=" << ONEWIRE_PIN << "\n";
}
