//
// Analog to Digital Converter
//

#include <abstract_task.h>
#include <definitions.h>

#include <ADS1X15.h> // ADS1x15
#include <I2CBus.h>
#include <Wire.h> // I2C

#include "ADC.h"
#include "DAC.h"
#include "DriverDisplayC.h"

extern I2CBus i2cBus;
extern ADC adc;

void ADC::re_init() { ADC::init(); }

void ADC::init() {

  // instantiate the devices with their corresponding address
  ads_addrs[0] = 0; // I2C_ADDRESS_ADS1x15_0;
  ads_addrs[1] = 0; // I2C_ADDRESS_ADS1x15_1;
  ads_addrs[2] = I2C_ADDRESS_ADS1x15_2;

  for (int idx = 0; idx < NUM_ADC_DEVICES; idx++) {
    if (ads_addrs[idx] == 0) {
      continue;
    }
    xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
    adss[idx] = ADS1115(ads_addrs[idx]);

    printf("  Initializing ADC[%d] with address 0x%x ...", idx, ads_addrs[idx]);
    bool result = adss[idx].begin();
    if (!result) {
      printf(" ERROR: Wrong ADS1x15 address 0x%x.\n", ads_addrs[idx]);
      xSemaphoreGive(i2cBus.mutex);
      continue;
    }
    printf("\n");
    // set gain amplifier value
    // 2/3x gain +/- 6.144V
    // 1 bit = 3mV (ADS1015) / 0.1875mV (ADS1115)
    adss[idx].setGain(0);

    // conversion factor:
    // bit-value -> mV: 2/3x gain +/- 6.144V
    // 1 bit = 3mV (ADS1015) 0.1875mV (ADS1115)
    float multiplier = adss[idx].toVoltage(1); // voltage factor

    printf("    Max voltage=%f with multiplier=%f\n", adss[idx].getMaxVoltage(), multiplier);
    // read all inputs & report
    for (int i = 0; i < 4; i++) {
      int16_t value = adss[idx].readADC(i);
      printf("      [ADS1x15] AIN%d --> %d: %fmV\n", i, value, multiplier * value);
    }
    adss[idx].setDataRate(3);
    xSemaphoreGive(i2cBus.mutex);

    adjustMin_acceleration_recuperation();
    printf("[v] ADC[%d] initialized.\n", idx);
  }
}

void ADC::adjustMin_acceleration_recuperation() {
  ads_min_acceleration = adc.read(ADC::Pin::STW_ACC) + minAdjustGap;
  ads_min_recuperation = adc.read(ADC::Pin::STW_DEC) + minAdjustGap;
  accDisplayLast = 0;
  justInited = false;
  printf("MIN values set recuperation: %5d, acceleration: %5d\n", ads_min_recuperation, ads_min_acceleration);
}

void ADC::adjustMax_acceleration_recuperation() {
  ads_max_acceleration = adc.read(ADC::Pin::STW_ACC) + maxAdjustGap;
  ads_max_recuperation = adc.read(ADC::Pin::STW_DEC) + maxAdjustGap;
  accDisplayLast = 0;
  justInited = false;
  printf("MAX values set recuperation: %5d, acceleration: %5d\n", ads_max_recuperation, ads_max_acceleration);
}

int16_t ADC::read(ADC::Pin port) {

  int idx = port >> 4;
  int pin = port & 0xf;
  // debug_printf("index: %d, pin: %d\n", index, pin);

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  int16_t value = ADC::adss[idx].readADC(pin);
  xSemaphoreGive(i2cBus.mutex);

  return value;
}

int ADC::normalize(int value, int maxValue) { return (int)(maxValue * value / 1024); }

int ADC::normalize2(int value, int maxValue, int min, int max) {
  int delta = max - min;
  int v = value - min;
  int retValue = (int)(v * maxValue / delta);
  if (abs(retValue) < 5) {
    retValue = 0;
  }
  return retValue;
}

int ADC::read_adc_acceleration_recuperation() {
  int16_t accelNorm = 0;
  int16_t recupNorm = 0;
  int16_t accDisplay = 0;

  int16_t valueRec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);
  // debug_printf("v0: %5d\tv1: %5d\n", valueRec, valueAcc);
  if (valueRec < ads_min_recuperation || valueRec > ads_max_recuperation) {
    // filter artefacts
    valueRec = recupLast;
  }

  if (valueAcc < ads_min_acceleration || valueAcc > ads_max_acceleration) {
    // filter artefacts
    valueAcc = accelLast;
  }

  bool accelChanged = abs(accelLast - valueAcc) > 3;
  bool recupChanged = abs(recupLast - valueRec) > 3;

  if (accelChanged | recupChanged | justInited) {
    if (recupChanged) {
      recupLast = valueRec;
      recupNorm = adc.normalize2(valueRec, 99, ads_min_recuperation, ads_max_recuperation);
    }
    // priority controll: recuperation wins
    if (recupNorm > 0) {
      accDisplay = -recupNorm;
    } else {
      if (accelChanged) {
        accelLast = valueAcc;
        accelNorm = adc.normalize2(valueAcc, 99, ads_min_acceleration, ads_max_acceleration);
      }
      accDisplay = accelNorm;
    }

    if (accDisplay != accDisplayLast) {
      accDisplayLast = (int)((accDisplayLast * 0 + accDisplay) / 1);
      debug_printf("Recu (v0):  %5d --> %3d | Accel (v1): %5d --> %3d | "
                   "ACCEL-DISPLAY: %d\n",
                   valueRec, recupNorm, valueAcc, accelNorm, accDisplayLast);
    }
  }
  justInited = false;
  return accDisplayLast;
}
