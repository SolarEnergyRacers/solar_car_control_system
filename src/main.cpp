/*
 *  TODO: add description & license
 */

// standard library includes
#include <stdio.h>

// FreeRTOS / Arduino
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Arduino.h>

// esp32 includes
#include <driver/gpio.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "sdkconfig.h"

// local includes
#include "definitions.h"

// external libs
#include <Wire.h>


// local libs
#include <Adafruit_ADS1015.h>
Adafruit_ADS1015 ads; // forAdafruit_ADS1115 ads;

#include <DS18B20.h>
DS18B20 ds(ONEWIRE_PIN);

// add C linkage definition
extern "C" {
    void app_main(void);
}

void blink_task(void *pvParameter) {
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(LED_BUILTIN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(LED_BUILTIN, GPIO_MODE_OUTPUT);
    while (1) {
        /* Blink off (output low) */
        gpio_set_level(LED_BUILTIN, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(LED_BUILTIN, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void chip_info(void *pvParameter) {

    // print chip information
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}

void restart(void *pvParameter) {

    // count time down
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // actual restart
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}

void init_i2c(void){
    // init i2c wire library
    Wire.begin(I2C_SDA, I2C_SCL, I2C_FREQ);
    // init mutex
    i2c_mutex = xSemaphoreCreateBinary();
    xSemaphoreGive(i2c_mutex);
}

void init_adc(void){
    // init library
    ads.begin();

    /*
     * set gain amplifier value                                       ADS1015  ADS1115
     *                                                                -------  -------
     * ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
     * ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
     * ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
     * ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
     * ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
     * ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
     */
    ads.setGain(GAIN_TWOTHIRDS);
}

void read_adc(void *pvParameter){

    while(1){

        // bit -> mV: 2/3x gain +/- 6.144V  1 bit = 3mV (ADS1015) 0.1875mV (ADS1215)
        float multiplier = 3.0f;

        int16_t adc0 = ads.readADC_SingleEnded(0);
        //vTaskDelay(50 / portTICK_PERIOD_MS);

        int16_t adc1 = ads.readADC_SingleEnded(1);
        //vTaskDelay(50 / portTICK_PERIOD_MS);

        int16_t adc2 = ads.readADC_SingleEnded(2);
        //vTaskDelay(50 / portTICK_PERIOD_MS);

        int16_t adc3 = ads.readADC_SingleEnded(3);
        printf("Reading ADS1x15 ADC inputs:\n");
        printf("AIN0: %fmV\n", multiplier*adc0);
        printf("AIN1: %fmV\n", multiplier*adc1);
        printf("AIN2: %fmV\n", multiplier*adc2);
        printf("AIN3: %fmV\n", multiplier*adc3);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void init_ds(){

    printf("[OneWire] num devices on bus: %d\n", ds.getNumberOfDevices());

    while (ds.selectNext()) {
        switch (ds.getFamilyCode()) {
            case MODEL_DS18S20:
                printf("Model: DS18S20/DS1820");
                break;
            case MODEL_DS1822:
                printf("Model: DS1822");
                break;
            case MODEL_DS18B20:
                printf("Model: DS18B20");
                break;
            default:
                printf("Unrecognized Device");
                break;
        }
        printf(" ");

        uint8_t address[8];
        ds.getAddress(address);

        printf("Address:");
        for (uint8_t i = 0; i < 8; i++) {

            printf(" %d", address[i]);
        }
        printf(" ");

        printf("Resolution: %d", ds.getResolution());
        printf(" ");

        printf("Power Mode: ");
        if (ds.getPowerMode()) {
            printf("External");
        } else {
            printf("Parasite");
        }
        printf("\n");
    }
}

void read_ds(void *pvParameter){

    while(1){

        while(ds.selectNext()){

            printf("Temperature: %fC / %fF\n", ds.getTempC(), ds.getTempF());
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {

    // init arduino library
    initArduino();

    init_i2c();
    init_adc();
    init_ds();

    // report chip info
    chip_info(NULL);

    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    xTaskCreate(&read_adc, "read_adc_task", 1800, NULL, 5, NULL);
    xTaskCreate(&read_ds, "read_ds_task", 1800, NULL, 5, NULL);

}
