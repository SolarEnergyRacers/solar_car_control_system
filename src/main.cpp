/*
 *  SER - Solar Car Control System - Main Routine for setting up tasks, initialize devices, ..
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
#include <Temp.h>
#include <Display.h>
#include <SDCard.h>

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
        xTaskCreate(&read_ds_demo_task, "read_ds_task", 2500, NULL, 5, NULL);
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
        xTaskCreate(&write_sdcard_demo_task, "write_sdcard_task", 2500, NULL, 5, NULL);
    }
    if(INT_ON) {
        // register interrupt handler
        register_gpio_interrupt();
        xTaskCreate(&int_report, "int_report_task", 2500, NULL, 5, NULL);
    }
    if(DISPLAY_ON){
        init_display();
        xTaskCreate(&draw_display_demo_task, "display_task", 2500, NULL, 5, NULL);
    }
}
