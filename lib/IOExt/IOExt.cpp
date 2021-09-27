//
// PCF8574 I/O Extension over I2C
//

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

  for(int i = 0; i < PCF8574_NUM_DEVICES; i++){
    if(IOExt[i].begin()){
        printf("Initialization of IOExt0%u failed.\n", i);
    } else {
        printf("Initialization of IOExt0%u successful.\n", i);
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

  // read all
  PCF8574::DigitalInput dra[PCF8574_NUM_DEVICES];

#ifdef USE_LEGACY_PIN_READ
  for(int i = 0; i < PCF8574_NUM_DEVICES; i++){
    dra[i] = IOExt[i].digitalReadAll();
  }
#elif
    for(int i = 0; i < PCF8574_NUM_DEVICES; i++){
        dra[i].p0 = IOExt[i].digitalRead(0);
        dra[i].p1 = IOExt[i].digitalRead(1);
        dra[i].p2 = IOExt[i].digitalRead(2);
        dra[i].p3 = IOExt[i].digitalRead(3);
        dra[i].p4 = IOExt[i].digitalRead(4);
        dra[i].p5 = IOExt[i].digitalRead(5);
        dra[i].p6 = IOExt[i].digitalRead(6);
        dra[i].p7 = IOExt[i].digitalRead(7);
    }
#endif
  xSemaphoreGive(i2cBus.mutex);

  // TODO: handle all 
}

void IOExt::setMode(Pin port, uint8_t mode){

  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  IOExt[idx].pinMode(pin, mode);
  xSemaphoreGive(i2cBus.mutex);

}

void IOExt::set(Pin port, bool value) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  IOExt[idx].digitalWrite(pin, value);
  xSemaphoreGive(i2cBus.mutex);
}

int IOExt::get(Pin port) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  int value = IOExt[idx].digitalRead(pin);
  xSemaphoreGive(i2cBus.mutex);

  return value;
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