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

void ADC::re_init() { ADC::init(); }

void ADC::init() {
  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  // instantiate the devices with their corresponding address
  ads[0] = ADS1015(I2C_ADDRESS_ADS1x15_0);
  ads[1] = ADS1015(I2C_ADDRESS_ADS1x15_1);
  ads[2] = ADS1015(I2C_ADDRESS_ADS1x15_2);

  // init library
  for (auto ads : ADC::ads) {

    ads.begin();

    // set gain amplifier value
    // 2/3x gain +/- 6.144V
    // 1 bit = 3mV (ADS1015) / 0.1875mV (ADS1115)
    ads.setGain(0);

    // conversion factor:
    // bit-value -> mV: 2/3x gain +/- 6.144V
    // 1 bit = 3mV (ADS1015) 0.1875mV (ADS1115)
    float multiplier = ads.toVoltage(1); // voltage factor

    printf("Max voltage: %f\n", ads.getMaxVoltage());
    // read all inputs & report
    for (int i = 0; i < 4; i++) {
      int16_t value = ads.readADC(i);
      printf("[ADS1x15] AIN%d --> %d: %fmV\n", i, value, multiplier * value);
    }
  }
  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end
}

int16_t ADC::read(ADC::Pin port) {
  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  int16_t value = ADC::ads[port >> 4].readADC(port & 0xf);
  // TODO: should re return value depending on pin? (i.e. MOTOR_SPEED returns actual speed)

  xSemaphoreGive(i2cBus.mutex);
  // CRITICAL SECTION I2C: end

  return value;
}

int ADC::normalize(int value, int maxValue) { return (int)(maxValue * value / 1024); }

extern ADC adc;

void read_adc_acceleration_recuperation(void *pvParameter) {

  int accelLast = 0, recupLast = 0;

  while (1) {
    int16_t accel = 0;
    int16_t recup = 0;
    int16_t accDisplay = 0;

    int16_t value0 = adc.read(ADC::Pin::STW_ACC);
    int16_t value1 = adc.read(ADC::Pin::STW_DEC);

    bool accelChanged = abs(accelLast - value0) > 10;
    bool recupChanged = abs(recupLast - value1) > 10;

    if (accelChanged | recupChanged) {

      if (accelChanged) {
        accelLast = value0;
        accel = adc.normalize(value0, 100);
      }
      if (recupChanged) {
        recupLast = value1;
        recup = adc.normalize(value1, 100);
      }
      // priority controll: recuperation wins
      if (recup > 0) {
        accDisplay = -recup;
        accel = 0;
      } else {
        accDisplay = accel;
        recup = 0;
      }
      // write console log
      printf("Acceleration: %4d --> %4d | Recuperation:  %4d --> %4d | "
             "ACCEL-DISPLAY: %d\n",
             value0, accel, value1, recup, accDisplay);
      //            // write driver display info // TODO: reactivate whenever we can get the display instance similar to extern ADC adc;
      //            write_acceleration(accDisplay);
      //            arrow_increase(accel > 0 ? true : false);
      //            arrow_decrease(recup > 0 ? true : false);
      //            // write motor acceleration and recuperation values
      //            set_pot(accel, pot_chan::POT_CHAN0);
      //            set_pot(recup, pot_chan::POT_CHAN1);
    }

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}