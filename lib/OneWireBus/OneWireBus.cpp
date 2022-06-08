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

string OneWireBus::init(void) {
  bool hasError = false;
  console << "[  ] Init 'OneWireBus'...\n";

  mutex = xSemaphoreCreateMutex();
  oneWire = OneWire(ONEWIRE_PIN);
  xSemaphoreGive(mutex);

  return fmt::format("[{}] OneWireBus initialized.  ONEWIRE_PIN={}", hasError ? "--" : "ok", ONEWIRE_PIN);
}
