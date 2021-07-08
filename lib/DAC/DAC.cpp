//
// Digital to Analog Converter
//

#include <definitions.h>

#include <Helper.h>
#include <I2CBus.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h> // I2C
#include <inttypes.h>
#include <stdio.h>

#include "DAC.h"
#include "DriverDisplayC.h"

#define BASE_ADDR_CMD 0xA8

extern I2CBus i2cBus;

void DAC::re_init() {
    init();
}

uint8_t DAC::get_cmd(pot_chan channel) {
  uint8_t command = BASE_ADDR_CMD;
  switch (channel) {
  case POT_CHAN0:
    command |= 0x01;
    break;
  case POT_CHAN1:
    command |= 0x10;
    break;
  case POT_CHAN_ALL:
    command |= 0x11;
    break;
  default:
    command |= 0x01;
    break;
  }
  return command;
}

void DAC::set_pot(uint8_t val, pot_chan channel) {

  // setup command
  uint8_t command = get_cmd(channel);

  // printf("Write motor potentiometer %02x to %d\n", command, val);

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  Wire.beginTransmission(I2C_ADDRESS_DS1803);
  Wire.write(command);
  Wire.write(val); // first pot value
  if (channel == POT_CHAN_ALL) {
    Wire.write(val); // second pot value
  }
  Wire.endTransmission();

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end
}

uint16_t DAC::get_pot(pot_chan channel) {

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  Wire.requestFrom(I2C_ADDRESS_DS1803, 2); // request 2 bytes
  uint8_t pot0 = Wire.read();              // get pot0
  uint8_t pot1 = Wire.read();              // get pot1

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end

  if (channel == POT_CHAN_ALL) {
    return pot0 | (pot1 << 8);
  } else if (channel == POT_CHAN0) {
    return pot0;
  } else { // POT_CHAN1
    return pot1;
  }
}

void DAC::init() {}
