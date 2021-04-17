//
// Digital to Analog Converter
//

#include "../../include/definitions.h"

#include <Helper.h>
#include <I2CBus.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Arduino.h>
#include <Wire.h> // I2C
#include <inttypes.h>
#include <stdio.h>

#include "DAC.h"
#include "DriverDisplay.h"

#define BASE_ADDR_CMD 0xA8

void dac_demo_task(void *pvParameter) {

  // // polling loop
  // uint8_t val = 0;
  // while (1) {

  //   // set potentiometer value
  //   printf("set potentiometer value=%d", val);
  //   set_pot(val, POT_CHAN0);
  //   set_pot(0xFF - val, POT_CHAN1); // negate

  //   // sleep for 100ms
  //   vTaskDelay(100 / portTICK_PERIOD_MS);

  //   // increment timer
  //   val += 10;
  // }
}

void dac_user_input_demo_task(void *pvParameter) {

  // // polling loop
  // uint8_t val = 0;
  // int input;
  // int n = 10;
  // char input_str[n];

  // printf("start dac task\n");

  while (1) {

    // // read user input
    // fgets_stdio_blocking(input_str, n);

    // printf("fgets read: %s\n", input_str);

    // sscanf(input_str, "%d", &input);

    // if (input >= 0 && input < 256) {
    //   val = input;
    //   printf("set dac to %d\n", val);
    // } else {
    //   printf("dac: input value out of bound: %d\n", input);
    // }

    // // set potentiometer value
    // printf("set potentiometer value=%d\n", val);
    // set_pot(val, POT_CHAN0);

    // sleep for 10s
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void init_dac() {
  // nothing to do, i2c bus is getting initialized externally
  printf("dac task inited\n");
}

uint8_t get_cmd(pot_chan channel) {
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

void set_pot(uint8_t val, pot_chan channel) {

  // setup command
  uint8_t command = get_cmd(channel);

  printf("Write motor potentiometer %02x to %d\n", command, val);

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  Wire.beginTransmission(I2C_ADDRESS_DS1803);
  Wire.write(command);
  Wire.write(val); // first pot value
  if (channel == POT_CHAN_ALL) {
    Wire.write(val); // second pot value
  }
  Wire.endTransmission();

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end
}

uint16_t get_pot(pot_chan channel) {

  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  Wire.requestFrom(I2C_ADDRESS_DS1803, 2); // request 2 bytes
  uint8_t pot0 = Wire.read();              // get pot0
  uint8_t pot1 = Wire.read();              // get pot1

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end

  if (channel == POT_CHAN_ALL) {
    return pot0 | (pot1 << 8);
  } else if (channel == POT_CHAN0) {
    return pot0;
  } else { // POT_CHAN1
    return pot1;
  }
}
