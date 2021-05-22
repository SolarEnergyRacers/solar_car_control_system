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
#include "Template.h"


#include "LocalFunctionsAndDevices.h"

// add C linkage definition
extern "C" {
void app_main(void);
}

void app_main(void) {

  class Template template0;
  template0.init();
  printf("Template: %d\n", template0.getValue());

  // init arduino library
  initArduino();

  // init serial output for console
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

  scan_i2c_devices();

  // ---- init modules ----
  if (BLINK_ON) {
  }
  if (DISPLAY_LARGE_ON) {
    init_driver_display();
  }
  if (DISPLAY_LARGE_INDICATOR_ON) {
    init_indicator();
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
    init_ioext();
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

  // ---- create tasks ----
  if (DISPLAY_ON) {
    xTaskCreate(&draw_display_demo_task, "display_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (DISPLAY_LARGE_ON) {
    xTaskCreate(&driver_display_task, "driver_display_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (DISPLAY_LARGE_INDICATOR_ON) {
    xTaskCreate(&indicator_task, "indicator_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (BLINK_ON) {
    xTaskCreate(&blink_demo_task, "blink_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (ADC_ON) {
    xTaskCreate(&read_adc_demo_task, "read_adc_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (DS_ON) {
    xTaskCreate(&read_ds_demo_task, "read_ds_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (GYRO_ACC_ON) {
    xTaskCreate(&read_gyro_acc_demo_task, "read_gyro_acc_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (PWM_ON) {
    xTaskCreate(&update_pwm_demo_task, "update_pwm_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (RTC_ON) {
    xTaskCreate(&read_rtc_demo_task, "read_adc_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (SD_ON) {
    xTaskCreate(&write_sdcard_demo_task, "write_sdcard_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (INT_ON) {
    xTaskCreate(&int_report_demo_task, "int_report_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (SIMULATOR_ON) {
    xTaskCreate(&simulator_task, "simulator_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (IOEXT_ON) {
    xTaskCreate(&io_ext_demo_task, "io_extension_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (DAC_ON) {
  }
  if (COMMANDHANDLER_ON) {
    xTaskCreate(&command_handler_task, "command_handler_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (SERIAL_ON) {
    xTaskCreate(&serial_demo_task, "serial_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (CAN_ON) {
    xTaskCreate(&read_can_demo_task, "can_task",
                CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
}
