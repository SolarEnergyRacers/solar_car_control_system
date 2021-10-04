//
// PCF8574 I/O Extension over I2C
//

#include <stdio.h>

#include <PCF8574.h> // PCF8574
#include <Wire.h>    // I2C
#include <definitions.h>

#include <I2CBus.h>

#include "IOExt.h"

extern I2CBus i2cBus;

void IOExt::re_init() { init(); }

void IOExt::init() {

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    printf("    Init IOExt %u...\n", devNr);
    if (IOExt[devNr].begin()) {
      printf("[x] Initialization of IOExt %u failed.\n", devNr);
    } else {
      for (int pin = 0; pin < PCF8574_NUM_PORTS; pin++) {
        int port = (devNr << 4) + pin;
        int idx = devNr * 8 + pin;
        printf("\tport 0x%02x, mode: %s --> value=%d (%s)\n", port, pins[idx].mode != OUTPUT ? "INPUT " : "OUTPUT", pins[idx].value,
               pins[idx].name.c_str());
        IOExt[devNr].pinMode(pin, pins[idx].mode);
      }
      printf("[v] Initialization of IOExt %u successful.\n", devNr);
    }
  }

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end
}

void IOExt::exit(void) {
  // TODO
}

void IOExt::handleIoInterrupt() {
  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

#ifdef USE_LEGACY_PIN_READ
  // read all
  PCF8574::DigitalInput dra[PCF8574_NUM_DEVICES];
  for (int i = 0; i < PCF8574_NUM_DEVICES; i++) {
    dra[i] = IOExt[i].digitalReadAll();
  }
#else
  list<void (*)()> pinHandlerList;
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    for (int pin = 0; pin < PCF8574_NUM_PORTS; pin++) {
      int idx = devNr * 8 + pin;
      if (pins[idx].mode != OUTPUT) {
        pins[idx].value = IOExt[devNr].digitalRead(pin);
        if (pins[idx].handlerFunction != NULL) {
          pinHandlerList.push_back(pins[idx].handlerFunction);
        }
      }
    }
  }
#endif
  xSemaphoreGive(i2cBus.mutex);
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    printf("0x%02x: ", devNr);
    for (int pin = 0; pin < PCF8574_NUM_PORTS; pin++) {
      string color = "";
      int idx = devNr * 8 + pin;
      if (pins[idx].mode == OUTPUT)
        color = "\033[1;31m";
      printf(" %s%d\033[0;39m", color.c_str(), pins[idx].value);
      if ((idx + 1) % 8 == 0)
        printf(" | ");
      else if ((idx + 1) % 4 == 0)
        printf(" - ");
    }
  }
  printf("\n");

  pinHandlerList.unique();
  for (void (*pinHandler)() : pinHandlerList) {
    pinHandler();
  }
  pinHandlerList.clear();

  // for (int idx = 0; idx < 32; idx++) {
  //   if (pins[idx].mode != OUTPUT && pins[idx].value == 0 && pins[idx].handlerFunction != NULL)
  //     pins[idx].handlerFunction(pins[idx].port);
  // }
}

extern IOExt ioExt;
extern Indicator indicator;

int IOExt::getIdx(int port) { return (port >> 4) * 8 + (port & 0x0F); }
// known pin handler
void batteryOnOffHandler() { printf("Battery %s\n", (ioExt.pins[0].value == 1 ? "On" : "Off")); }
void pvOnOffHandler() { printf("PV %s\n", (ioExt.pins[1].value == 1 ? "On" : "Off")); }
void mcOnOffHandler() { printf("MC %s\n", (ioExt.pins[2].value == 1 ? "On" : "Off")); }
void indicatorHandler() {
  int left = ioExt.pins[ioExt.getIdx(0x20)].value;
  int right = ioExt.pins[ioExt.getIdx(0x21)].value;
  //printf("idx: %d -- %d\n", ioExt.getIdx(0x20), ioExt.getIdx(0x21));
  if (left == 0 && right == 0)
    indicator.setIndicator(INDICATOR::WARN);
  else if (left == 0)
    indicator.setIndicator(INDICATOR::LEFT);
  else if (right == 0)
    indicator.setIndicator(INDICATOR::RIGHT);
}
// end pin handler

void IOExt::setMode(int port, uint8_t mode) {

  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  IOExt[idx].pinMode(pin, mode);
  xSemaphoreGive(i2cBus.mutex);
}

void IOExt::set(int port, bool value) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  IOExt[idx].digitalWrite(pin, value);
  xSemaphoreGive(i2cBus.mutex);
}

int IOExt::get(int port) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  int value = IOExt[idx].digitalRead(pin);
  xSemaphoreGive(i2cBus.mutex);

  printf("0x%02x [%d|%d]: %d\n", port, idx, pin, value);
  return value;
}

void IOExt::getAll(Pin *pins, int maxCount) {
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    printf("0x%02x: ", devNr);
    for (int pin = 0; pin < PCF8574_NUM_PORTS; pin++) {
      int id = (devNr << 4) + pin;
      if (pins[id].value != 0) {
        printf("%s: %d\n", pins[id].name.c_str(), pins[id].value);
      }
    }
    printf("\n");
  }
}

volatile bool IOExt::ioInterruptRequest = false;

void IOExt::task() {

  // polling loop
  while (1) {
    // handle input interrupts
    if (ioInterruptRequest && !isInInterruptHandler) {
      isInInterruptHandler = true;
      handleIoInterrupt();
      ioInterruptRequest = false;
      isInInterruptHandler = false;
    }
    // sleep
    vTaskDelay(taskSleep / portTICK_PERIOD_MS);
  }
}
