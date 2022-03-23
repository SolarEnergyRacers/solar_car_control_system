//
// Digital to Analog Converter
//

#include <definitions.h>

#include <fmt/core.h>
#include <fmt/printf.h>
#include <inttypes.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Arduino.h>

#include <CarState.h>
#include <Console.h>
#include <DAC.h>
#include <DriverDisplay.h>
#include <Helper.h>
#include <I2CBus.h>
#include <SPI.h>
#include <Wire.h> // I2C

#define BASE_ADDR_CMD 0xA8

extern CarState carState;
extern DriverDisplay driverDisplay;
extern I2CBus i2cBus;
extern Console console;

void DAC::re_init() { reset_and_lock_pot(); }

void DAC::init() {
  console << "[?] Setup 'DAC'...\n";
  reset_and_lock_pot();
  string s = fmt::format("[v] DAC initialized with I2C_ADDRESS_DS1803={:x}.\n", I2C_ADDRESS_DS1803);
  console << s;
  driverDisplay.print(s.c_str());
}

void DAC::lock() {
  //#SAVETY#: acceleration lock
  isLocked = true;
  carState.AccelerationLocked = true;
};

uint8_t DAC::get_cmd(pot_chan channel) {
  uint8_t command = BASE_ADDR_CMD;
  switch (channel) {
  case POT_CHAN0:
    command |= 0x1;
    break;
  case POT_CHAN1:
    command |= 0x2;
    break;
  case POT_CHAN_ALL:
    command |= 0x3;
    break;
  default:
    command |= 0x1;
    break;
  }
  return command;
}

void DAC::reset_and_lock_pot() {
  lock();
  uint8_t command = get_cmd(POT_CHAN_ALL);
  xSemaphoreTakeT(i2cBus.mutex);
  Wire.beginTransmission(I2C_ADDRESS_DS1803);
  Wire.write(command);
  Wire.write(0); // first pot value
  Wire.write(0); // second pot value
  Wire.endTransmission();
  xSemaphoreGive(i2cBus.mutex);
  // debug_printf("Write motor potentiometer [0x%02x/Ch%d] 0x%02x to %d -- reread: %d\n", I2C_ADDRESS_DS1803, POT_CHAN_ALL, command, 0,
  //              get_pot(POT_CHAN_ALL));
  console << fmt::sprintf("Write motor potentiometer [0x%02x/Ch%d] 0x%02x to %d -- reread: %d\n", I2C_ADDRESS_DS1803, POT_CHAN_ALL, command,
                          0, get_pot(POT_CHAN_ALL));
}

void DAC::set_pot(uint8_t val, pot_chan channel) {
  //#SAVETY#: acceleration lock
  if (isLocked) {
    if (carState.PaddlesJustAdjusted && carState.AccelerationDisplay == 0) {
      unlock();
      carState.AccelerationLocked = false;
      string s = fmt::format("DAC unlocked.\n");
      console << s;
      if (driverDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
        driverDisplay.print(s.c_str());
      }
    } else {
      string s = fmt::format("Motor potentiometer locked!\n");
      console << s;
      if (driverDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
        driverDisplay.print(s.c_str());
      }
      return;
    }
  }
  // if (val > DAC_MAX)
  //   val = DAC_MAX;
  // setup command
  uint8_t command = get_cmd(channel);
  uint8_t oldValue = get_pot(channel);
  if (oldValue != val) {
    //#SAVETY#: Reset constant mode on deceleration paddel touched
    if (channel == POT_CHAN1 || channel == POT_CHAN_ALL) {
      carState.ConstantModeOn = false;
    }
    xSemaphoreTakeT(i2cBus.mutex);
    Wire.beginTransmission(I2C_ADDRESS_DS1803);
    Wire.write(command);
    Wire.write(val); // first pot value
    if (channel == POT_CHAN_ALL) {
      Wire.write(val); // second pot value
    }
    Wire.endTransmission();
    xSemaphoreGive(i2cBus.mutex);
    // debug_printf("Write motor potentiometer [0x%02x/Ch%d] 0x%02x to %d -- reread: %d\n", I2C_ADDRESS_DS1803, channel, command, val,
    //              get_pot(channel));
    console << fmt::sprintf("Write motor potentiometer [0x%02x/Ch%d] 0x%02x to %d -- reread: %d\n", I2C_ADDRESS_DS1803, channel, command,
                            val, get_pot(channel));
  }
}

uint16_t DAC::get_pot(pot_chan channel) {
  uint8_t pot0 = 0; // get pot0
  uint8_t pot1 = 0; // get pot1
  xSemaphoreTakeT(i2cBus.mutex);
  Wire.requestFrom(I2C_ADDRESS_DS1803, 2); // request 2 bytes
  pot0 = Wire.read();
  pot1 = Wire.read();
  xSemaphoreGive(i2cBus.mutex);

  if (channel == POT_CHAN_ALL) {
    return pot0 | (pot1 << 8);
  } else if (channel == POT_CHAN0) {
    return pot0;
  } else { // POT_CHAN1
    return pot1;
  }
}
