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
#include <OneWire.h> // 1-Wire
#include <Wire.h> // I2C

// local libs
#include "ADS1X15.h"
ADS1015 ads(0x48); // ADS1115 ADS(0x48);


#include <DallasTemperature.h>
OneWire oneWire(ONEWIRE_PIN);
DallasTemperature ds(&oneWire);

// gyro & acc
#include <BMI088.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

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

    // set gain amplifier value
    ads.setGain(0); // 2/3x gain +/- 6.144V  1 bit = 3mV (ADS1015) / 0.1875mV (ADS1115)
}

void read_adc(void *pvParameter){

    while(1){

        // bit -> mV: 2/3x gain +/- 6.144V  1 bit = 3mV (ADS1015) 0.1875mV (ADS1215)
        float multiplier = ads.toVoltage(1);  // voltage factor

        for(int i = 0; i < 4; i++){
            int16_t value = ads.readADC(i);
            printf("[ADS1x15] AIN%d: %fmV\n", i, multiplier * value);
        }

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void init_ds(){

    ds.begin();

    printf("[OneWire] num devices on bus: %d\n", ds.getDeviceCount());


    DeviceAddress device_addr;

    oneWire.reset_search();
    while (oneWire.search(device_addr)) {

        printf("[DS18B20] Address:");
        for (uint8_t i = 0; i < 8; i++) {

            printf(" %d", device_addr[i]);
        }
        printf(" ");

        printf("Resolution: %d", ds.getResolution(device_addr));
        printf(" ");

        printf("Power Mode: ");
        if (ds.isParasitePowerMode()) {
            printf("External");
        } else {
            printf("Parasite");
        }
        printf("\n");
    }
}

void read_ds(void *pvParameter){

    // polling loop
    while(1){

        // request all temperature sensor readings
        ds.requestTemperatures();

        // print all results
        for(int i = 0; i < ds.getDeviceCount(); i++){
            printf("[DS18B20] Temperature: %fC / %fF\n", ds.getTempCByIndex(i), ds.getTempFByIndex(i));
        }

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void init_gyro_acc(void){

    if (bmi088.isConnection()) {
        bmi088.initialize();
        printf("BMI088 is connected");
    } else {
        printf("BMI088 is not connected");
    }

}

void init_onewire(void){

}

void read_gyro_acc(void *pvParameter){

    float ax = 0, ay = 0, az = 0;
    float gx = 0, gy = 0, gz = 0;
    int16_t temp = 0;

    while(1){

        bmi088.getAcceleration(&ax, &ay, &az);
        bmi088.getGyroscope(&gx, &gy, &gz);
        temp = bmi088.getTemperature();

        printf("[BMI088] ax=%f, ay=%f, az=%f\n", ax, ay, az);
        printf("[BMI088] gx=%f, gy=%f, gz=%f\n", gx, gy, gz);
        printf("[BMI088] temperature=%d\n", temp);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void init_pwm(void){
    pwm.begin();

    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(1600);  // max pwm frequency

}

void update_pwm(void *pvParameter){

    while(1){

        for (int i = 0; i < 4096; i += 128) {
            for (int output=0; output < 16; output++) {
                pwm.setPWM(output, 0, i);
            }
            printf("[PCA9685] value=%d\n", i);

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

void app_main(void) {

    // init arduino library
    initArduino();

    // init buses
    init_i2c();
    init_onewire();

    // init modules
    init_adc();
    init_ds();
    init_gyro_acc();
    init_pwm();

    // report chip info
    chip_info(NULL);

    // create all tasks
    xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    xTaskCreate(&read_adc, "read_adc_task", 2000, NULL, 5, NULL);
    xTaskCreate(&read_ds, "read_ds_task", 2000, NULL, 5, NULL);
    xTaskCreate(&read_gyro_acc, "read_gyro_acc_task", 2000, NULL, 5, NULL);
    xTaskCreate(&update_pwm, "update_pwm_task", 2000, NULL, 5, NULL);

}
