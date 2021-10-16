/*
 *  SER - Solar Car Control System - Main Routine for setting up tasks,
 * initialize devices, ..
 *
 * clang style:
 *    ./extras/format.sh
 */

// standard libraries
#include <stdio.h>

// FreeRTOS / Arduino
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

// project variables
#include <sdkconfig.h>

// local definitions
#include <definitions.h>

// local libs
#include <ADC.h>
#include <CANBus.h>
#include <CarSpeed.h>
#include <CmdHandler.h>
#include <DAC.h>
#include <EngineerDisplay.h>
#include <DriverDisplay.h>
#include <GPIO.h>
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
#include <Temp.h>
#include <iostream>
#include <string>
#include <system.h>

#include <LocalFunctionsAndDevices.h>
#include <abstract_task.h>

#include <CarStatePin.h>
#include <CarStateValue.h>
#include <CarState.h>

// add C linkage definition
extern "C" {
void app_main(void);
}

using namespace std;
// using namespace DriverDisplay;

ADC adc;
CanBus can; // TODO: gets a linking-error if we set CAN_ON to true
CarSpeed carSpeed;
CarState carState;
CmdHandler cmdHandler;
DAC dac;
EngineerDisplay engDisp;
DriverDisplay dd;
GPInputOutput gpio; // I2C Interrupts
GyroAcc gyroAcc;
I2CBus i2cBus;
Indicator indicator; // DISPLAY_LARGE_INDICATOR_ON
IOExt ioExt;
OneWireBus oneWireBus;
PWM pwm;
RTC rtc;
SDCard sdCard;
SPIBus spiBus;
Temp ds; // temperature
Uart uart; // SERIAL

bool startOk = true;
bool systemOk = false;

void app_main(void) {

  // init arduino library
  initArduino();

  // init serial output for  console

  Serial.begin(115200);
  delay(2000);
  printf("\n--------------------\n");
  Serial.println("esp32dev + free RTOS");
  printf("--------------------\n");

  // report chip info
  chip_info();
  printf("--------------------\n");

  cout << carState.print("Recent values:");

  cout << carState.serialize("") << endl;
  printf("========================================================================================\n");

  // init buses
  oneWireBus.init();
  i2cBus.init();
  spiBus.init();

  // ---- init modules ----
  if (DRIVER_DISPLAY_ON) {
    dd.init();
  }
  if (BLINK_ON) {
  }
  if (DISPLAY_LARGE_INDICATOR_ON) {
    indicator.init();
  }
  if (COMMANDHANDLER_ON) {
    cmdHandler.init();
  }
  if (ADC_ON) {
    adc.init();
  }
  if (DS_ON) {
    ds.init();
  }
  if (GYRO_ACC_ON) {
    gyroAcc.init();
  }
  if (PWM_ON) {
    pwm.init();
  }
  if (RTC_ON) {
    rtc.init();
  }
  if (SD_ON) {
    sdCard.init();
  }
  if (INT_ON) {
    gpio.init();
    gpio.register_gpio_interrupt();
  }
  if (ENGINEER_DISPLAY_ON) {
    engDisp.init();
  }
  if (IOEXT_ON) {
    ioExt.init();
  }
  if (DAC_ON) {
    dac.init();
  }
  if (SERIAL_ON) {
    uart.init();
  }
  if (CAN_ON) {
    can.init();
  }

  if (!startOk) {
    printf("ERROR in init sequence(s). System halted!\n");
    exit(0);
  }
  printf("\n-----------------------------------------------------------------\n");
  printf("Startup sequence(s) successful. System creating FreeRTOS tasks...\n");
  printf("-----------------------------------------------------------------\n");
  dd.print("\n---------------------------------\n");
  dd.print("Startup sequence(s) successful.\nSystem creating FreeRTOS tasks...\n");
  dd.print("---------------------------------\n");

  // ---- create tasks ----
  if (ENGINEER_DISPLAY_ON) {
    engDisp.create_task();
  }
  if (DRIVER_DISPLAY_ON) {
    dd.create_task();
    dd.print("[v] " + dd.getName() + "task initialized.\n");
  }

  if (DISPLAY_LARGE_INDICATOR_ON) {
    indicator.create_task();
    dd.print("[v] " + indicator.getName() + "task initialized.\n");
  }
  // if (BLINK_ON) { // not activated
  //   printf(" - blink_demo_task\n");
  //   xTaskCreate(&blink_demo_task, "blink_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  // }
 
  if (DS_ON) {
    xTaskCreate(&read_ds_demo_task, "read_ds_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (GYRO_ACC_ON) {
    xTaskCreate(&read_gyro_acc_demo_task, "read_gyro_acc_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (PWM_ON) {
    pwm.create_task();
    dd.print("[v] " + pwm.getName() + "task initialized.\n");
  }
  if (RTC_ON) {
    rtc.create_task();
    dd.print("[v] " + rtc.getName() + "task initialized.\n");
  }
  if (SD_ON) {
    xTaskCreate(&write_sdcard_demo_task, "write_sdcard_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (INT_ON) {
    gpio.create_task();
    dd.print("[v] " + gpio.getName() + "task initialized.\n");
  }
  if (IOEXT_ON) {
    ioExt.create_task();
    dd.print("[v] " + ioExt.getName() + "task initialized.\n");
  }
  if (DAC_ON) {
    dac.init();
    printf(" - DAC DAC DAC\n");
  }
  if (COMMANDHANDLER_ON) {
    cmdHandler.create_task();
    dd.print("[v] " + cmdHandler.getName() + "task initialized.\n");
  }
  if (SERIAL_ON) {
    printf(" - serial_demo_task\n");
    // xTaskCreate(&serial_demo_task, "serial_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (CAN_ON) {
    printf(" - read_can_demo_task\n");
    xTaskCreate(&read_can_demo_task, "can_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }

  if (CARSPEED_ON) {
    carSpeed.create_task();
    dd.print("[v] " + carSpeed.getName() + "task initialized.\n");
  }

  systemOk = true;

  printf("-----------------------------------------------------------------\n");
  printf("Creating FreeRTOS tasks successful. System running.\n");
  printf("-----------------------------------------------------------------\n\n");
  dd.print("\n----------------------------------------------------\n");
  dd.print("FreeRTOS tasks created successfully. System running.\n");
  dd.print("----------------------------------------------------\n");

  if (DRIVER_DISPLAY_ON) {
    dd.setScreen0Mode();
  }
}
