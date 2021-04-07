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
int valueLast0 = 9999;
float valueLast1 = 9999.9;
float valueLast2 = 9999.9;
float valueLast3 = 9999.9;

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

int _normalize(int value, int maxValue)
{
    //return (int)(10 * value / 4096);
    return (int)(maxValue * value / 1100);
}

float _normalize(float value, float maxValue)
{
    //return (int)(10 * value / 4096);
    return (maxValue * value / 1100);
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

    printf("Max voltage: %f\n", ads.getMaxVoltage());
    // read all inputs & report
    for (int i = 0; i < 4; i++)
    {
        int16_t value = _read_adc(i);
        printf("[ADS1x15] AIN%d --> %d: %fmV\n", i, value, multiplier * value);
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

        int16_t value0 = ads.readADC(0);
        int16_t value1 = ads.readADC(1);
        int16_t value2 = ads.readADC(2);
        int16_t value3 = ads.readADC(3);

        xSemaphoreGive(i2c_mutex);
        // CRITICAL SECTION I2C: end

        if (abs(valueLast0 - value0) > 10)
        {
            valueLast0 = value0;
            int acc = _normalize(value0, 10);
            printf("Acceleration: %d --> %d\n", value0, acc);
            write_acceleration(acc);
        }
        if (abs(valueLast1 - value1) > 10)
        {
            valueLast1 = value1;
            float acc = _normalize(value1, 999.9);
            printf("Battery: %d --> %6.1f\n", value1, acc);
            write_bat(acc);
        }
        if (abs(valueLast2 - value2) > 10)
        {
            valueLast2 = value2;
            float acc = _normalize(value2, 9999.9);
            printf("PV: %d --> %6.1f\n", value2, 9999.9/2 - acc);
            write_pv(9999.9/2 - acc);
        }
        if (abs(valueLast3 - value3) > 10)
        {
            valueLast3= value3;
            float acc = _normalize(value3, 9999.9);
            printf("Motor: %d --> %6.1f\n", value3, 9999.9/2 - acc);
            write_motor(9999.9/2 - acc);
        }
        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
