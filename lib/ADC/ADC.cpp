//
// Analog to Digital Converter
//

#include <definitions.h>
#include <abstract_task.h>

#include <ADS1X15.h> // ADS1x15
#include <I2CBus.h>
#include <Wire.h> // I2C

#include "ADC.h"
#include "DAC.h"
#include "DriverDisplayC.h"



void ADC::re_init() {
    this->init();
}

void ADC::init() {
    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    // init library
    for (auto ads : this->ads) {

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
    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end
}

int16_t ADC::read(ADC::Pin port){
    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    int16_t value = ads[port >> 4].readADC(port & 0xf);
    // TODO: should re return value depending on pin? (i.e. MOTOR_SPEED returns actual speed)

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end

    return value;
}

int ADC::normalize(int value, int maxValue) {
    return (int)(maxValue * value / 1024);
}
