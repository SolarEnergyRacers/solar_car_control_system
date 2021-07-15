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
  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  // instantiate the devices with their corresponding address
  adcs[2] = ADS1015(I2C_ADDRESS_ADS1x15_0);
  adcs[1] = ADS1015(I2C_ADDRESS_ADS1x15_1);
  adcs[0] = ADS1015(I2C_ADDRESS_ADS1x15_2);

  // init library
  // for (int idx = 0; idx < NUM_ADC_DEVICES; idx++) {
  for (int idx = 0; idx < 1; idx++) {
    // for (auto ads : ADC::adcs) {
    ADS1015 ads = adcs[idx];
    printf("  Initializing ADS[%d] ...\n", idx);
    bool result = ads.begin();
    if (!result) {
      printf("ERROR\n");
      continue;
    }
    // set gain amplifier value
    // 2/3x gain +/- 6.144V
    // 1 bit = 3mV (ADS1015) / 0.1875mV (ADS1115)
    ads.setGain(0);

    // conversion factor:
    // bit-value -> mV: 2/3x gain +/- 6.144V
    // 1 bit = 3mV (ADS1015) 0.1875mV (ADS1115)
    float multiplier = ads.toVoltage(1); // voltage factor

    printf("    Max voltage: %f\n", ads.getMaxVoltage());
    // read all inputs & report
    for (int i = 0; i < 4; i++) {
      int16_t value = ads.readADC(i);
      printf("    [ADS1x15] AIN%d --> %d: %fmV\n", i, value, multiplier * value);
    }
    printf("[v] ADS[%d] initialized.\n", idx);
  }
  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end
}

int16_t ADC::read(ADC::Pin port) {

  int index = port >> 4;
  int pin = port & 0xf;
  // debug_printf("index: %d, pin: %d\n", index, pin);

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  int16_t value = ADC::adcs[index].readADC(pin);
  xSemaphoreGive(i2cBus.mutex);

  return value;
}

int ADC::normalize(int value, int maxValue) { return (int)(maxValue * value / 1024); }
int ADC::normalize2(int value, int maxValue, int min, int max) {
  int delta = max - min;
  int v = value - min;
  return (int)(v * maxValue / delta);
}

int16_t accelLast = 0;
int16_t recupLast = 0;
int16_t accDisplayLast = 0;

int ADC::read_adc_acceleration_recuperation() {
  int16_t accel = 0;
  int16_t recup = 0;
  int16_t accDisplay = 0;

  int16_t value0 = adc.read(ADC::Pin::STW_ACC);
  int16_t value1 = adc.read(ADC::Pin::STW_DEC);

  if (value0 < 0 || value0 > 2000 || value1 < 0 || value1 > 2000) {
    // filter artefacts
    value0 = accelLast;
    value1 = recupLast;
  }

  bool accelChanged = abs(accelLast - value0) > 10;
  bool recupChanged = abs(recupLast - value1) > 10;

  if (accelChanged | recupChanged) {

    if (recupChanged) {
      recupLast = value1;
      // recup = adc.normalize(value1, 100);
      recup = adc.normalize2(value1, 10, 350, 1000);
    }
    // priority controll: recuperation wins
    if (recup > 0) {
      accDisplay = -recup;
    } else {
      if (accelChanged) {
        accelLast = value0;
        // accel = adc.normalize(value0, 100);
        accel = adc.normalize2(value0, 10, 350, 1000);
      }
      accDisplay = accel;
    }
    debug_printf("Recuperation:  %4d --> %4d | Acceleration: %4d --> %4d | ACCEL-DISPLAY: %d\n", value1, recup, value0, accel, accDisplay);
    //            // write driver display info // TODO: reactivate whenever we can get the display instance similar to extern ADC adc;
    //            arrow_increase(accel > 0 ? true : false);
    //            arrow_decrease(recup > 0 ? true : false);
    //            // write motor acceleration and recuperation values
    //            set_pot(accel, pot_chan::POT_CHAN0);
    //            set_pot(recup, pot_chan::POT_CHAN1);
   accDisplayLast = (int)((accDisplayLast * 4 + accDisplay) / 5);
  }
  return accDisplayLast;
}
