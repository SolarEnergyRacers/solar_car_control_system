//
// Analog to Digital Converter
//
#include <definitions.h>

#include <fmt/core.h>
#include <fmt/printf.h>
#include <inttypes.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <ADS1X15.h>
#include <I2CBus.h>
#include <Wire.h> // I2C

#include <ADC.h>
#include <CarState.h>
#include <Console.h>
#include <DAC.h>
#include <DriverDisplay.h>
#include <Helper.h>
#include <abstract_task.h>

extern Console console;
extern CarState carState;
extern I2CBus i2cBus;
extern ADC adc;
extern DAC dac;
extern DriverDisplay driverDisplay;

using namespace std;

void ADC::re_init() { init(); }

void ADC::init() {
  // instantiate the devices with their corresponding address
  ads_addrs[0] = I2C_ADDRESS_ADS1x15_0;
  ads_addrs[1] = I2C_ADDRESS_ADS1x15_1;
  ads_addrs[2] = I2C_ADDRESS_ADS1x15_2;
  string s;
  for (int idx = 0; idx < NUM_ADC_DEVICES; idx++) {
    console << fmt::format("[?] Init 'ADC[{}]' with address {:#04x} ...", idx, ads_addrs[idx]);
    xSemaphoreTakeT(i2cBus.mutex);
    adss[idx] = ADS1115(ads_addrs[idx]);

    bool result = adss[idx].begin();
    if (!result) {
      console << fmt::format(" ERROR: Wrong ADS1x15 address {:#04x}.\n", ads_addrs[idx]);
      xSemaphoreGive(i2cBus.mutex);
      continue;
    }
    console << "\n";
    // set gain amplifier value
    // 2/3x gain +/- 6.144V
    // 1 bit = 3mV (ADS1015) / 0.1875mV (ADS1115)
    adss[idx].setGain(0);
    adss[idx].setMode(1);
    adss[idx].setDataRate(3);

    // conversion factor:
    // bit-value -> mV: 2/3x gain +/- 6.144V
    // 1 bit = 3mV (ADS1015) 0.1875mV (ADS1115)
    float multiplier = adss[idx].toVoltage(1); // voltage factor
    xSemaphoreGive(i2cBus.mutex);
    console << "    Max voltage=" << adss[idx].getMaxVoltage() << " with multiplier=" << multiplier << "\n";
    // read all inputs & report
    for (int i = 0; i < 4; i++) {
      xSemaphoreTakeT(i2cBus.mutex);
      int16_t value = adss[idx].readADC(i);
      xSemaphoreGive(i2cBus.mutex);
      console << "      [ADS1x15] AIN" << i << " --> " << value << ": " << multiplier * value << "mV\n";
    }

    string s = fmt::format("[v] ADC[{}] at 0x{:x} initialized.\n", idx, ads_addrs[idx]);
    console << s;
    driverDisplay.print(s.c_str());
  }
}

void ADC::exit(void) {
  // TODO
}

int16_t ADC::read(ADC::Pin port) {
  int idx = port >> 4;
  int pin = port & 0xf;
  int16_t value = 0;

  xSemaphoreTakeT(i2cBus.mutex);
  value = ADC::adss[idx].readADC(pin);
  xSemaphoreGive(i2cBus.mutex);

  // if (port == STW_ACC || port == STW_DEC || port == MOTOR_SPEED) {
  //    console << fmt::sprintf("port 0x%02x: index: 0x%x, pin: 0x%x => value=%d\n", port, idx, pin, value);
  // }
  return value;
}

float ADC::get_multiplier(Pin port) {
  int devNr = port >> 4;
  return adss[devNr].toVoltage(1);
}

void ADC::task() {
  // polling loop
  while (1) {
    // ADC0
    carState.MOTOR_SPEED = read(MOTOR_SPEED);
    carState.BAT_CURRENT = read(BAT_CURRENT);
    carState.MOTOR_CURRENT = read(MOTOR_CURRENT);
    carState.PV_CURRENT = read(PV_CURRENT);
    // ADC1
    carState.BAT_VOLTAGE = read(BAT_VOLTAGE);
    carState.MOTOR_VOLTAGE = read(MOTOR_VOLTAGE);
    carState.REFERENCE_CELL = read(REFERENCE_CELL);
    // ADC2 (steering wheel)
    carState.STW_ACC = read(STW_ACC);
    carState.STW_DEC = read(STW_DEC);

    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
