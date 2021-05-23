/*
 *  SER - Solar Car Control System - Main Routine for setting up tasks,
 * initialize devices, ..
 */

// standard libraries
#include <stdio.h>

// FreeRTOS / Arduino
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

// project variables
#include "sdkconfig.h"

// local definitions
#include "definitions.h"

// local libs
#include <ADC.h>
#include <CANBus.h>
#include <CmdHandler.h>
#include <DAC.h>
#include <Display.h>
#include <DriverDisplay.h>
#include <Gyro_Acc.h>
#include <I2CBus.h>
#include <IOExt.h>
#include <Indicator.h>
#include <OneWireBus.h>
#include <PWM.h>
#include <RTC.h>
#include <SDCard.h>
#include <SPIBus.h>
#include <Serial.h>
#include <Simulator.h>
#include <Temp.h>
#include <gpio.h>
#include <system.h>

#include "LocalFunctionsAndDevices.h"

// add C linkage definition
extern "C" {
void app_main(void);
}

void app_main(void) {

  bool startOk = true;

  // init arduino library
  initArduino();

  // init serial output for  console 

  Serial.begin(115200);
  delay(2000);
  printf("\n--------------------\n");
  Serial.println("esp32dev + free RTOS\n");
  printf("--------------------\n");

  // report chip info
  chip_info();

  // init buses
  init_onewire();
  init_i2c();
  init_spi();

  // ---- init modules ----
  if (BLINK_ON) {
  }
  if (DISPLAY_LARGE_ON) {
    startOk &= init_driver_display();
  }
  if (DISPLAY_LARGE_INDICATOR_ON) {
    startOk &= init_indicator();
  }
  if (COMMANDHANDLER_ON) {
    init_command_handler();
  }
  if (ADC_ON) {
    init_adc();
  }
  if (DS_ON) {
    init_ds();
  }
  if (GYRO_ACC_ON) {
    init_gyro_acc();
  }
  if (PWM_ON) {
    init_pwm();
  }
  if (RTC_ON) {
    init_rtc();
  }
  if (SD_ON) {
    init_sdcard();
  }
  if (INT_ON) {
    register_gpio_interrupt();
  }
  if (DISPLAY_ON) {
    init_display();
  }
  if (IOEXT_ON) {
    init_IOExt2();
  }
  if (DAC_ON) {
    init_dac();
  }
  if (SERIAL_ON) {
    init_serial();
  }
  if (SIMULATOR_ON) {
    init_simulator();
  }
  if (CAN_ON) {
    init_can();
  }

  if (!startOk) {
    printf("ERROR in init sequence(s). System halted!\n");
    exit(0);
  }
  printf("-----------------------------------------------------------------\n");
  printf("Startup sequence(s) successful. System creating FreeRTOS tasks...\n");
  printf("-----------------------------------------------------------------\n");

  // ---- create tasks ----
  if (DISPLAY_ON) {
    printf(" - draw_display_demo_task\n");
    xTaskCreate(&draw_display_demo_task, "draw_display_demo_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (DISPLAY_LARGE_ON) {
    printf(" - driver_display_task\n");
    xTaskCreate(&driver_display_task, "driver_display_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (DISPLAY_LARGE_INDICATOR_ON) {
    printf(" - indicator_task\n");
    xTaskCreate(&indicator_task, "indicator_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (BLINK_ON) {
    printf(" - blink_demo_task\n");
    xTaskCreate(&blink_demo_task, "blink_demo_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (ADC_ON) {
    printf(" - read_adc_demo_task\n");
    xTaskCreate(&read_adc_demo_task, "read_adc_demo_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (DS_ON) {
    printf(" - read_ds_demo_task\n");
    xTaskCreate(&read_ds_demo_task, "read_ds_demo_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (GYRO_ACC_ON) {
    printf(" - read_gyro_acc_demo_task\n");
    xTaskCreate(&read_gyro_acc_demo_task, "read_gyro_acc_demo_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (PWM_ON) {
    printf(" - update_pwm_demo_task\n");
    xTaskCreate(&update_pwm_demo_task, "update_pwm_demo_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (RTC_ON) {
    printf(" - read_rtc_demo_task\n");
    xTaskCreate(&read_rtc_demo_task, "read_adc_demo_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (SD_ON) {
    printf(" - write_sdcard_demo_task\n");
    xTaskCreate(&write_sdcard_demo_task, "write_sdcard_demo_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (INT_ON) {
    printf(" - int_report_demo_task\n");
    xTaskCreate(&int_report_demo_task, "int_report_demo_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (SIMULATOR_ON) {
    printf(" - simulator_task\n");
    xTaskCreate(&simulator_task, "simulator_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (IOEXT_ON) {
    printf(" - IOExt2_task\n");
    xTaskCreate(&IOExt2_task, "IOExt2_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (DAC_ON) {
    printf(" - DAC DAC DAC\n");
  }
  if (COMMANDHANDLER_ON) {
    printf(" - command_handler_task\n");
    xTaskCreate(&command_handler_task, "command_handler_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (SERIAL_ON) {
    printf(" - serial_demo_task\n");
    xTaskCreate(&serial_demo_task, "serial_demo_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (CAN_ON) {
    printf(" - read_can_demo_task\n");
    xTaskCreate(&read_can_demo_task, "can_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  printf("-----------------------------------------------------------------\n");
  printf("Creating FreeRTOS tasks successful. System running.\n");
  printf(
      "-----------------------------------------------------------------\n\n");
}
