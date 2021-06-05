//
// Analog to Digital Converter
//

#include "../../include/definitions.h"

#include <ADS1X15.h> // ADS1x15
#include <I2CBus.h>
#include <Wire.h> // I2C

#include "ADC.h"
#include "DAC.h"
#include "DriverDisplay.h"

#define ADC_NUM_PORTS 4

ADS1015 ads(
    I2C_ADDRESS_ADS1x15); // for ADS1115 use: ADS1115 ads(I2C_ADDRESS_ADS1x15);

float multiplier = 0;
int16_t valueLast0 = 9999;
float valueLast1 = 9999.9;
float valueLast2 = 9999.9;
float valueLast3 = 9999.9;

int16_t accelLast = 0;
int16_t recupLast = 0;

int _normalize(int value, int maxValue) {
  // return (int)(10 * value / 4096);
  return (int)(maxValue * value / 1024);
}

void init_adc() {
  // CRITICAL SECTION I2C: start
  xSemaphoreTake(i2c_mutex, portMAX_DELAY);

  // init library
  ads.begin();

  // set gain amplifier value
  // 2/3x gain +/- 6.144V
  // 1 bit = 3mV (ADS1015) / 0.1875mV (ADS1115)
  ads.setGain(0);

  // conversion factor:
  // bit-value -> mV: 2/3x gain +/- 6.144V
  // 1 bit = 3mV (ADS1015) 0.1875mV (ADS1115)
  multiplier = ads.toVoltage(1); // voltage factor

  printf("Max voltage: %f\n", ads.getMaxVoltage());
  // read all inputs & report
  for (int i = 0; i < 4; i++) {
    int16_t value = ads.readADC(i);
    printf("[ADS1x15] AIN%d --> %d: %fmV\n", i, value, multiplier * value);
  }

  xSemaphoreGive(i2c_mutex);
  // CRITICAL SECTION I2C: end
}

void read_adc_demo_task(void *pvParameter) {

  while (1) {
    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    int16_t value0 = ads.readADC(0);
    int16_t value1 = ads.readADC(1);
    int16_t value2 = ads.readADC(2);
    int16_t value3 = ads.readADC(3);

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end

    if (abs(valueLast0 - value0) > 10) {
      valueLast0 = value0;
      int acc = _normalize(value0, 100);
      printf("Acceleration: %d --> %d\n", value0, acc);
      write_acceleration(acc);
    }
    if (abs(valueLast1 - value1) > 10) {
      valueLast1 = value1;
      float acc = _normalize(value1, 999.9);
      printf("Battery: %d --> %6.1f\n", value1, acc);
      write_bat(acc);
    }
    if (abs(valueLast2 - value2) > 10) {
      valueLast2 = value2;
      float acc = _normalize(value2, 9999.9);
      printf("PV: %d --> %6.1f\n", value2, 9999.9 / 2 - acc);
      write_pv(9999.9 / 2 - acc);
    }
    if (abs(valueLast3 - value3) > 10) {
      valueLast3 = value3;
      float acc = _normalize(value3, 9999.9);
      printf("Motor: %d --> %6.1f\n", value3, 9999.9 / 2 - acc);
      write_motor(9999.9 / 2 - acc);
    }
    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void read_adc_acceleration_recuperation(void *pvParameter) {

  while (1) {
    int16_t accel = 0;
    int16_t recup = 0;
    int16_t accDisplay = 0;

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    int16_t value0 = ads.readADC(0);
    int16_t value1 = ads.readADC(1);

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end

    bool accelChanged = abs(accelLast - value0) > 10;
    bool recupChanged = abs(recupLast - value1) > 10;

    if (accelChanged | recupChanged) {

      if (accelChanged) {
        accelLast = value0;
        accel = _normalize(value0, 100);
      }
      if (recupChanged) {
        recupLast = value1;
        recup = _normalize(value1, 100);
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
      // write driver display info
      write_acceleration(accDisplay);
      arrow_increase(accel > 0 ? true : false);
      arrow_decrease(recup > 0 ? true : false);
      // write motor acceleration and recuperation values
      set_pot(accel, pot_chan::POT_CHAN0);
      set_pot(recup, pot_chan::POT_CHAN1);
    }

    // sleep for 1s
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
