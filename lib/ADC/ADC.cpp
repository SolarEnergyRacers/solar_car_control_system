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
#include <Console.h>
#include <DAC.h>
#include <DriverDisplay.h>
#include <Helper.h>
#include <abstract_task.h>

extern Console console;
extern I2CBus i2cBus;
extern ADC adc;
extern DAC dac;
extern DriverDisplay driverDisplay;

using namespace std;

void ADC::init() {
  // instantiate the devices with their corresponding address
  ads_addrs[0] = I2C_ADDRESS_ADS1x15_0;
  ads_addrs[1] = I2C_ADDRESS_ADS1x15_1;
  ads_addrs[2] = I2C_ADDRESS_ADS1x15_2;
  string s;
  for (int idx = 0; idx < NUM_ADC_DEVICES; idx++) {
    s = fmt::format("[?] Init 'ADC[{}]' with address {:#04x} ...", idx, ads_addrs[idx]);
    console << s;
    xSemaphoreTakeT(i2cBus.mutex);
    adss[idx] = ADS1115(ads_addrs[idx]);

    bool result = adss[idx].begin();
    if (!result) {
      s = fmt::format(" ERROR: Wrong ADS1x15 address {:#04x}.\n", ads_addrs[idx]);
      console << s;
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
    console << "    Max voltage=" << adss[idx].getMaxVoltage() << " with multiplier=" << multiplier << "\n";
    // read all inputs & report
    for (int i = 0; i < 4; i++) {
      int16_t value = adss[idx].readADC(i);
      console << "      [ADS1x15] AIN" << i << " --> " << value << ": " << multiplier * value << "mV\n";
    }
    xSemaphoreGive(i2cBus.mutex);

    string s = fmt::format("[v] ADC[{}] at 0x{:x} initialized.\n", idx, ads_addrs[idx]);
    console << s;
    driverDisplay.print(s.c_str());
  }
}

int16_t ADC::read(ADC::Pin port) {
  int idx = port >> 4;
  int pin = port & 0xf;
  int16_t value = 0;
  xSemaphoreTakeT(i2cBus.mutex);
  value = ADC::adss[idx].readADC(pin);
  xSemaphoreGive(i2cBus.mutex);

  // console << fmt::sprintf("index: 0x%x, pin: 0x%x => value=%d\n", idx, pin, value);
  return value;
}

float ADC::get_multiplier(Pin port) {
  int devNr = port >> 4;
  return adss[devNr].toVoltage(1);
}
