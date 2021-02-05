//
// Analog to Digital Converter
//

#include "../../include/definitions.h"

#include <I2C.h>
#include "ADC.h"

#include <Wire.h> // I2C
#include <ADS1X15.h> // ADS1x15

#define NUM_PORTS 4


ADS1015 ads(I2C_ADDRESS_ADS1x15); // for ADS1115 use: ADS1115 ads(I2C_ADDRESS_ADS1x15);

void init_adc() {
    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    // init library
    ads.begin();

    // set gain amplifier value
    ads.setGain(0); // 2/3x gain +/- 6.144V  1 bit = 3mV (ADS1015) / 0.1875mV (ADS1115)

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end
}

int16_t read_adc(int port){

    // check port number
    if(port < 0 || port >= NUM_PORTS){
        return -1;
    }

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    // read value
    int16_t value = ads.readADC(port);

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end

    return value;
}


void read_adc_demo_task(void *pvParameter) {

    while (1) {

        // conversion factor: bit-value -> mV: 2/3x gain +/- 6.144V  1 bit = 3mV (ADS1015) 0.1875mV (ADS1215)
        float multiplier = ads.toVoltage(1);  // voltage factor

        // read all inputs & report
        for (int i = 0; i < 4; i++) {
            int16_t value = read_adc(i);
            printf("[ADS1x15] AIN%d: %fmV\n", i, multiplier * value);
        }

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

