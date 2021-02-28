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

// project variables
#include "sdkconfig.h"

// local definitions
#include "definitions.h"

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
#include <DisplayLarge.h>
#include <SDCard.h>
#include <gpio.h>
#include <IOExt.h>

// (de-)activate functionality & devices
#define BLINK_ON true
#define ADC_ON true
#define DS_ON true
#define GYRO_ACC_ON true
#define PWM_ON true
#define RTC_ON true
#define INT_ON true
#define SD_ON true
#define DISPLAY_ON true
#define DISPLAY_LARGE_ON true
#define INT_ON false
#define IOEXT_ON false

// add C linkage definition
extern "C" {
    void app_main(void);
}

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
    if (BLINK_ON) {
        xTaskCreate(&blink_demo_task, "blink_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (ADC_ON) {
        init_adc();
        xTaskCreate(&read_adc_demo_task, "read_adc_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (DS_ON) {
        init_ds();
        xTaskCreate(&read_ds_demo_task, "read_ds_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (GYRO_ACC_ON) {
        init_gyro_acc();
        xTaskCreate(&read_gyro_acc_demo_task, "read_gyro_acc_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (PWM_ON) {
        init_pwm();
        xTaskCreate(&update_pwm_demo_task, "update_pwm_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (RTC_ON) {
        init_rtc();
        xTaskCreate(&read_rtc_demo_task, "read_adc_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (SD_ON) {
        init_sdcard();
        xTaskCreate(&write_sdcard_demo_task, "write_sdcard_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (INT_ON) {
        register_gpio_interrupt();
        xTaskCreate(&int_report_demo_task, "int_report_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (DISPLAY_ON) {
        init_display();
        xTaskCreate(&draw_display_demo_task, "display_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if (DISPLAY_LARGE_ON) {
        init_display_large();
        xTaskCreate(&draw_display_large_demo_task, "display_large_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
    if(IOEXT_ON){
        init_ioext();
        xTaskCreate(&io_ext_demo_task, "io_extension_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    }
}
