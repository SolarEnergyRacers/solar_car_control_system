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
//#include <CarState.h>
#include <Console.h>
#include <DAC.h>
#include <DriverDisplay.h>
#include <Helper.h>
#include <abstract_task.h>

extern Console console;
// extern CarState carState;
extern I2CBus i2cBus;
extern ADC adc;
extern DAC dac;
extern DriverDisplay driverDisplay;

using namespace std;

string ADC::re_init() { return init(); }

string ADC::init() {
  bool hasError = false;
  // instantiate the devices with their corresponding address
  ads_addrs[0] = I2C_ADDRESS_ADS1x15_0;
  ads_addrs[1] = I2C_ADDRESS_ADS1x15_1;
  ads_addrs[2] = I2C_ADDRESS_ADS1x15_2;
  for (int idx = 0; idx < NUM_ADC_DEVICES; idx++) {
    console << fmt::format("     Init 'ADC[{}]' with address {:#04x} ...", idx, ads_addrs[idx]);
    xSemaphoreTakeT(i2cBus.mutex);
    adss[idx] = ADS1115(ads_addrs[idx]);

    bool result = adss[idx].begin();
    if (!result) {
      xSemaphoreGive(i2cBus.mutex);
      console << fmt::format("\n        ERROR: Wrong ADS1x15 at address: {:#04x}.\n", ads_addrs[idx]);
      hasError = true;
      ads_addrs[idx] = 0;
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
    console << "        Max voltage=" << adss[idx].getMaxVoltage() << " with multiplier=" << multiplier << "\n";
    // read all inputs & report
    for (int i = 0; i < 4; i++) {
      xSemaphoreTakeT(i2cBus.mutex);
      int16_t value = adss[idx].readADC(i);
      xSemaphoreGive(i2cBus.mutex);
      console << "          [ADS1x15] AIN" << i << " --> " << value << ": " << multiplier * value << "mV\n";
    }
    console << fmt::format("     ok ADC[{}] at 0x{:x} inited.\n", idx, ads_addrs[idx]);
  }
  return fmt::format("[{}] ADC initialized.", hasError ? "--" : "ok");
}

void ADC::exit(void) {
  // TODO
}

int16_t ADC::read(ADC::Pin port) {
  int idx = port >> 4;
  int pin = port & 0xf;
  int16_t value = 0;

  if (ads_addrs[idx] != 0) {
    xSemaphoreTakeT(i2cBus.mutex);
    value = ADC::adss[idx].readADC(pin);
    xSemaphoreGive(i2cBus.mutex);
  }
  return value;
}

float ADC::get_multiplier(Pin port) {
  int devNr = port >> 4;
  if (ads_addrs[devNr] != 0) {
    return adss[devNr].toVoltage(1);
  } else {
    return 0;
  }
}

void ADC::task() {
  // polling loop
  while (1) {
    // ADC0
    MOTOR_SPEED = read(Pin::MOTOR_SPEED_PORT);
    BAT_CURRENT = read(Pin::BAT_CURRENT_PORT);
    MOTOR_CURRENT = read(Pin::MOTOR_CURRENT_PORT);
    PV_CURRENT = read(Pin::PV_CURRENT_PORT);
    // ADC1
    BAT_VOLTAGE = read(Pin::BAT_VOLTAGE_PORT);
    MOTOR_VOLTAGE = read(Pin::MOTOR_VOLTAGE_PORT);
    REFERENCE_CELL = read(Pin::REFERENCE_CELL_PORT);
    GREEN_LIGHT = read(Pin::GREEN_LIGHT_PORT);
    carState.GreenLight = GREEN_LIGHT < 7000 ? true : false;
    // ADC2 (steering wheel)
    STW_ACC = read(Pin::STW_ACC_PORT);
    STW_DEC = read(Pin::STW_DEC_PORT);

    if (verboseModeADC) {
      console << fmt::format("adc: speed {:3d} | acc {:5d} | dec {:5d} | batC {:5d} | motC {:5d}I | pvC {:5d} | batV {:5d} | motV {:5d} | "
                             "refV {:5d} | GreenLightb {:5d}\n",
                             MOTOR_SPEED, STW_ACC, STW_DEC, BAT_CURRENT, MOTOR_CURRENT, PV_CURRENT, BAT_VOLTAGE, MOTOR_VOLTAGE,
                             REFERENCE_CELL, GREEN_LIGHT);
    }

    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
