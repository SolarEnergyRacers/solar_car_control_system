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
    Wire.begin(I2C_SDA, I2C_SCL, I2C_FREQ);
}

void app_main(void) {

    // init arduino library
    initArduino();

    // report chip info
    chip_info(NULL);

    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
