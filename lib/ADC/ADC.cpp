//
// Analog to Digital Converter
//

#include "../../include/definitions.h"

#include <I2CBus.h>
#include <Wire.h>    // I2C
#include <ADS1X15.h> // ADS1x15

#include "ADC.h"
#include "DriverDisplay.h"

#define ADC_NUM_PORTS 4

ADS1015 ads(I2C_ADDRESS_ADS1x15); // for ADS1115 use: ADS1115 ads(I2C_ADDRESS_ADS1x15);

float multiplier = 0;
int accLast = 0;

int16_t _read_adc(int port)
{
    // check port number
    if (port < 0 || port >= ADC_NUM_PORTS)
    {
        return -1;
    }

    // read value
    int16_t value = ads.readADC(port);

    return value;
}

int _normalize(int value)
{
    //return (int)(10 * value / 4096);
    return (int)(10 * value / 1100);
}

void init_adc()
{
    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    // init library
    ads.begin();

    // set gain amplifier value
    ads.setGain(0); // 2/3x gain +/- 6.144V  1 bit = 3mV (ADS1015) / 0.1875mV (ADS1115)

    // conversion factor: bit-value -> mV: 2/3x gain +/- 6.144V  1 bit = 3mV (ADS1015) 0.1875mV (ADS1215)
    multiplier = ads.toVoltage(1); // voltage factor

    Serial.printf("Max voltage: %f\n", ads.getMaxVoltage());
    // read all inputs & report
    for (int i = 0; i < 4; i++)
    {
        int16_t value = _read_adc(i);
        Serial.printf("[ADS1x15] AIN%d --> %d: %fmV\n", i, value, multiplier * value);
    }

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end
}

void read_adc_demo_task(void *pvParameter)
{

    while (1)
    {
        // CRITICAL SECTION I2C: start
        xSemaphoreTake(i2c_mutex, portMAX_DELAY);

        int16_t value = ads.readADC(0);

        xSemaphoreGive(i2c_mutex);
        // CRITICAL SECTION I2C: end

        int acc = _normalize(value);
        if (abs(accLast - acc) > 0)
        {
            printf("Acceleration: %d --> %d\n", value, acc);
            write_acceleration(acc);
            accLast = acc;
        }
        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
