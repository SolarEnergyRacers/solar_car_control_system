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
#include <Display.h>
#include <DriverDisplay.h>
#include <ESP32Time.h>
#include <EngineerDisplay.h>
#include <GPIO.h>
#include <Gyro_Acc.h>
#include <I2CBus.h>
#include <IOExt.h>
#include <IOExt2.h>
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

#include <CarControl.h>
#include <CarState.h>
#include <CarStatePin.h>
#include <CarStateValue.h>

// add C linkage definition
extern "C" {
void app_main(void);
}

using namespace std;
// using namespace DriverDisplay;

ADC adc;
CanBus can; // TODO: gets a linking-error if we set CAN_ON to true
CarControl carControl;
CarSpeed carSpeed;
CarState carState;
CmdHandler cmdHandler;
DAC dac;
DriverDisplay driverDisplay;
EngineerDisplay engineerDisplay;
ESP32Time esp32time;
GPInputOutput gpio; // I2C Interrupts
GyroAcc gyroAcc;
I2CBus i2cBus;
Indicator indicator; // INDICATOR_ON
IOExt ioExt;
OneWireBus oneWireBus;
PWM pwm;
RTC rtc;
SDCard sdCard;
SPIBus spiBus;
Temp ds;   // temperature
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
  printf("esp32dev + free RTOS\n");
  printf("Solar Energy Car Races SER4, %s, %s\n", VERSION, VERSION_PUBLISHED);
  printf("--------------------\n");

  // report chip info
  printf("-chip info -------------------\n");
  chip_info();
  printf("-init bus systems ------------\n");
  // init buses
  spiBus.init();
  oneWireBus.init();
  i2cBus.init();
  printf("------------------------------\n");

  // ---- init modules ----
  if (ENGINEER_DISPLAY_ON) {
    engineerDisplay.init();
    engineerDisplay.create_task(5);
    engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::HALTED);
    engineerDisplay.print("[v] " + engineerDisplay.getName() + "task initialized, HALTED.\n");
    sleep(1);
  }
  if (DRIVER_DISPLAY_ON) {
    driverDisplay.init();
    driverDisplay.create_task(5);
    driverDisplay.set_DisplayStatus(DISPLAY_STATUS::CONSOLE);
    driverDisplay.print("[v] " + driverDisplay.getName() + "task initialized, CONSOLE.\n");
    sleep(1);
  }
  if (BLINK_ON) {
  }
  if (INDICATOR_ON) {
    indicator.init();
    indicator.setIndicator(INDICATOR::OFF);
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

  // ---- create tasks ----
  if (INDICATOR_ON) {
    indicator.create_task();
    driverDisplay.print("[v] " + indicator.getName() + "task initialized.\n");
  }
  if (DS_ON) {
    xTaskCreate(&read_ds_demo_task, "read_ds_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (GYRO_ACC_ON) {
    xTaskCreate(&read_gyro_acc_demo_task, "read_gyro_acc_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (PWM_ON) {
    pwm.create_task();
    driverDisplay.print("[v] " + pwm.getName() + "task initialized.\n");
  }
  if (RTC_ON) {
    RtcDateTime now = rtc.read_rtc_datetime();
    debug_printf("[RTC] current datetime: %02u/%02u/%04u %02u:%02u:%02u\n", now.Month(), now.Day(), now.Year(), now.Hour(), now.Minute(),
                 now.Second());
    esp32time.setTime(now.Second(), now.Minute(), now.Hour(), now.Day(), now.Month(), now.Year());
    driverDisplay.print("[v] " + rtc.getName() + " initialized, time in esp32 updated.\n");
  }
  if (SD_ON) {
    xTaskCreate(&write_sdcard_demo_task, "write_sdcard_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (INT_ON) {
    gpio.create_task();
    driverDisplay.print("[v] " + gpio.getName() + "task initialized.\n");
  }
  if (IOEXT_ON) {
    ioExt.create_task();
    ioExt.readAll();
    indicator.setIndicator(INDICATOR::OFF);
    carState.Indicator.set(INDICATOR::OFF);
    carState.ConstantModeOn.set(false);
    carState.Light.set(LIGHT::OFF);
    driverDisplay.print("[v] " + ioExt.getName() + "task initialized.\n");
  }
  if (DAC_ON) {
    dac.init();
    printf(" - DAC DAC DAC\n");
  }
  if (COMMANDHANDLER_ON) {
    cmdHandler.create_task();
    driverDisplay.print("[v] " + cmdHandler.getName() + "task initialized.\n");
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
    driverDisplay.print("[v] " + carSpeed.getName() + "task initialized.\n");
  }
  if (CARCONTROL_ON) {
    carControl.init();
    carControl.create_task();
    driverDisplay.print("[v] " + carControl.getName() + "task initialized.\n");
  }
  if (DRIVER_DISPLAY_ON) {
    driverDisplay.set_DisplayStatus(DISPLAY_STATUS::SETUPDRIVER);
    driverDisplay.print("[v] " + driverDisplay.getName() + "task initialized.\n");
    driverDisplay.print("Startup sequence(s) successful.\nSystem creating FreeRTOS tasks...\n");
  }
  systemOk = true;

  cout << "-----------------------------------------------------------------" << endl;
  cout << "Creating FreeRTOS tasks successful. System running." << endl;
  cout << "-----------------------------------------------------------------" << endl;
  driverDisplay.print("FreeRTOS tasks created successfully. System running.");
  driverDisplay.print("ok.");
  cout << endl << carState.print("Initial car state:") << endl;
}
