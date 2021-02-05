/*
 *  TODO: add description & license
 */

// standard libraries
#include <stdio.h>

// FreeRTOS / Arduino
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <Arduino.h>

// esp32 includes
#include <driver/gpio.h>

#include "sdkconfig.h"

// local definitions
#include "definitions.h"

// external libs
#include <OneWire.h> // 1-Wire
#include <Wire.h> // I2C
#include <SPI.h> // SPI

// local libs
#include <system.h>
#include <OneWireBus.h>
#include <I2CBus.h>
#include <SPIBus.h>
#include <ADC.h>
#include <Gyro_Acc.h>
#include <PWM.h>
#include <RTC.h>

#include <DallasTemperature.h> // DS18B20
OneWire oneWire(ONEWIRE_PIN);
DallasTemperature ds(&oneWire);


#include <SD.h> // sd card

#include <Adafruit_GFX.h> // graphics library
#include <Adafruit_SSD1305.h> // display
#define OLED_RESET 9
Adafruit_SSD1305 display(128, 64, &Wire, OLED_RESET);

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

void init_ds() {

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

void read_ds(void *pvParameter) {

    // polling loop
    while (1) {

        // request all temperature sensor readings
        ds.requestTemperatures();

        // print all results
        for (int i = 0; i < ds.getDeviceCount(); i++) {
            printf("[DS18B20] Temperature: %fC / %fF\n", ds.getTempCByIndex(i), ds.getTempFByIndex(i));
        }

        // sleep for 1s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


File dataFile;
void init_sdcard(void){

    if (!SD.begin(SPI_CS_SDCARD)) {
        printf("[SDCard] Initialization failed\n");
    } else {
        printf("[SDCard] Initialization successful\n");

        dataFile = SD.open("/datalog.txt", FILE_APPEND); //FILE_WRITE);

    }
}

void write_sdcard(void *pvParameter) {


    int counter = 0;

    while(1){

        if (dataFile) {
            dataFile.print(counter);
            dataFile.println("");
            printf("[SDCard] Write to sdcard: %d\n", counter++);
        } else {
            printf("[SDCard] Error opening file.\n");
        }
        // dataFile.close();
        dataFile.flush(); // ensure write-back

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile int interrupt_counter = 0;
void IRAM_ATTR handle_gpio_interrupt();
void register_gpio_interrupt(){

    printf("[HW Interrupt] Register gpio interrupt pin %d (falling edge)\n", GPIO_INTERRUPT_PIN);
    pinMode(GPIO_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(GPIO_INTERRUPT_PIN), handle_gpio_interrupt, FALLING);

}

void IRAM_ATTR handle_gpio_interrupt() {
    portENTER_CRITICAL_ISR(&mux);
    interrupt_counter++;
    portEXIT_CRITICAL_ISR(&mux);
}

void int_report(void *pvParameter){

    while(1){

        printf("[INT] Number of interrupts: %d\n", interrupt_counter);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void init_display(void){

    if (!display.begin(0x3C) ) {
        printf("[Display] Unable to initialize OLED screen.\n");
    } else {
        printf("[Display] Screen initialize successfully.\n");
    }

    // init done
    display.display(); // show splashscreen
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    display.clearDisplay();   // clears the screen and buffer



}


void draw_display(void *pvParameter){

    while(1){

        display.clearDisplay();   // clears the screen and buffer

        display.setTextSize(1);
        display.setTextWrap(false);
        display.setTextColor(WHITE);
        display.setCursor(0,0);

        for (uint8_t i=0; i < 168; i++) {
            if (i == '\n') continue;
            display.write(i);
            if ((i > 0) && (i % 21 == 0))
                display.println();
        }
        display.display();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#define BLINK_ON true
#define ADC_ON true
#define DS_ON true
#define GYRO_ACC_ON true
#define PWM_ON true
#define RTC_ON true
#define INT_ON true
#define SD_ON true
#define DISPLAY_ON true


void app_main(void) {

    // init arduino library
    initArduino();

    // report chip info
    chip_info();

    // init buses
    init_onewire();
    init_i2c();
    init_spi();

    // init modules & create tasks
    if(BLINK_ON){
        xTaskCreate(&blink_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    }
    if(ADC_ON){
        init_adc();
        xTaskCreate(&read_adc_demo_task, "read_adc_task", 2500, NULL, 5, NULL);
    }
    if(DS_ON) {
        init_ds();
        xTaskCreate(&read_ds, "read_ds_task", 2500, NULL, 5, NULL);
    }
    if(GYRO_ACC_ON){
        init_gyro_acc();
        xTaskCreate(&read_gyro_acc_demo_task, "read_gyro_acc_task", 2500, NULL, 5, NULL);
    }
    if(PWM_ON) {
        init_pwm();
        xTaskCreate(&update_pwm_demo_task, "update_pwm_task", 2500, NULL, 5, NULL);
    }
    if(RTC_ON) {
        init_rtc();
        xTaskCreate(&read_rtc_demo_task, "read_adc_task", 2500, NULL, 5, NULL);
    }
    if(SD_ON){
        init_sdcard();
        xTaskCreate(&write_sdcard, "write_sdcard_task", 2500, NULL, 5, NULL);
    }
    if(INT_ON) {
        // register interrupt handler
        register_gpio_interrupt();
        xTaskCreate(&int_report, "int_report_task", 2500, NULL, 5, NULL);
    }
    if(DISPLAY_ON){
        init_display();
        xTaskCreate(&draw_display, "display_task", 2500, NULL, 5, NULL);
    }
}
