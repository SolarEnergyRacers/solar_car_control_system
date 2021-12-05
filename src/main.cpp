/*
 *  SER - Solar Car Control System - Main Routine for setting up tasks,
 * initialize devices, ..
 *
 * clang style:
 *    ./extras/format.sh
 */

// standard libraries
#include <iostream>
#include <stdio.h>
#include <string>

// FreeRTOS / Arduino
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/FreeRTOSConfig.h>
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
#include <IOExt2.h>
#include <Indicator.h>
#include <MCP23017.h>
#include <OneWireBus.h>
#include <PWM.h>
#include <RTC.h>
#include <SDCard.h>
#include <SPIBus.h>
#include <Serial.h>
#include <Temp.h>
#include <system.h>

#include <LocalFunctionsAndDevices.h>
#include <abstract_task.h>

#include <CarControl.h>
#include <CarState.h>
#include <CarStatePin.h>

// add C linkage definition
extern "C" {
void app_main(void);
}

using namespace std;

ADC adc;
CanBus can; // TODO: gets a linking-error if we set CAN_ON to true
CAN_device_t CAN_cfg;
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
IOExt2 ioExt;
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
  delay(300);
  cout << endl;
  cout << "--------------------" << endl;
  cout << "esp32dev + free RTOS" << endl;
  cout << "Solar Energy Car Races SER4" << VERSION << " -- " << VERSION_PUBLISHED << endl;
  cout << "--------------------" << endl;

  // report chip info
  cout << "-chip info -------------------" << endl;
  chip_info();
  cout << "-init bus systems ------------" << endl;
  // init buses
  spiBus.init();
  oneWireBus.init();
  i2cBus.init();
  cout << "------------------------------" << endl;

  engineerDisplay.init();
  engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_CONSOLE);
  engineerDisplay.print("[v] " + engineerDisplay.getName() + " initialized, " + engineerDisplay.get_DisplayStatus_text() + ".\n");
  // ---- init modules ----
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
  if (IOEXT2_ON) {
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
    cout << "ERROR in init sequence(s). System halted!" << endl;
    exit(0);
  }

  engineerDisplay.print("Startup sequence(s) successful.\n");
  engineerDisplay.print("System creating FreeRTOS tasks...\n");
  cout << endl;
  cout << "-----------------------------------------------------------------" << endl;
  cout << "Startup sequence(s) successful. System creating FreeRTOS tasks..." << endl;
  cout << "-----------------------------------------------------------------" << endl << endl;

  // ---- create tasks ----
  if (INDICATOR_ON) {
    indicator.create_task();
    engineerDisplay.print("[v] " + indicator.getName() + " task initialized.\n");
  }
  if (DS_ON) {
    xTaskCreate(&read_ds_demo_task, "read_ds_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (GYRO_ACC_ON) {
    xTaskCreate(&read_gyro_acc_demo_task, "read_gyro_acc_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (PWM_ON) {
    pwm.create_task();
    engineerDisplay.print("[v] " + pwm.getName() + " task initialized.\n");
  }
  if (RTC_ON) {
    RtcDateTime now = rtc.read_rtc_datetime();
    debug_printf("[RTC] current datetime: %02u/%02u/%04u %02u:%02u:%02u\n", now.Month(), now.Day(), now.Year(), now.Hour(), now.Minute(),
                 now.Second());
    esp32time.setTime(now.Second(), now.Minute(), now.Hour(), now.Day(), now.Month(), now.Year());
    engineerDisplay.print("[v] " + rtc.getName() + " initialized, time in esp32 updated.\n");
  }
  if (SD_ON) {
    xTaskCreate(&write_sdcard_demo_task, "write_sdcard_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (INT_ON) {
    gpio.create_task();
    engineerDisplay.print("[v] " + gpio.getName() + " task initialized.\n");
  }

  if (DAC_ON) {
    dac.init();
    cout << " - DAC DAC DAC" << endl;
  }
  if (COMMANDHANDLER_ON) {
    cmdHandler.create_task();
    engineerDisplay.print("[v] " + cmdHandler.getName() + " task initialized.\n");
  }
  if (SERIAL_ON) {
    cout << " - serial_demo_task" << endl;
    xTaskCreate(&serial_demo_task, "serial_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (CAN_ON) {
    cout << " - read_can_demo_task" << endl;
    xTaskCreate(&read_can_demo_task, "can_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (CARSPEED_ON) {
    carSpeed.create_task();
    engineerDisplay.print("[v] " + carSpeed.getName() + " task initialized.\n");
  }
  if (CARCONTROL_ON) {
    carControl.init();
    carControl.create_task();
    engineerDisplay.print("[v] " + carControl.getName() + " task initialized.\n");
  }
  if (IOEXT2_ON) {
    carState.Indicator = INDICATOR::OFF;
    carState.ConstantModeOn = false;
    carState.ConstantMode = CONSTANT_MODE::SPEED;
    carState.Light = LIGHT::OFF;
    ioExt.create_task();
    engineerDisplay.print("[v] " + ioExt.getName() + " task initialized.\n");
    ioExt.readAll();
  }
  //--let the bootscreen visible for a moment ------------------
  int waitAtConsoleView = 5;
  engineerDisplay.print("\nready at ");
  engineerDisplay.print(esp32time.getDateTime().c_str());
  engineerDisplay.print(".\nWaiting for start of life display: ");
  while (waitAtConsoleView-- > 0) {
    engineerDisplay.print(to_string(waitAtConsoleView));
    sleep(1);
    engineerDisplay.print("-");
  }
  engineerDisplay.print("start");
  engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_HALTED);
  //------------------------------------------------------------
  if (ENGINEER_DISPLAY_ON) {
    engineerDisplay.create_task(10);
  }
  if (DRIVER_DISPLAY_ON) {
    driverDisplay.init();
    driverDisplay.set_DisplayStatus(DISPLAY_STATUS::DRIVER_SETUP);
    driverDisplay.create_task(16);
    cout << "[v] " << driverDisplay.getName() << " task initialized, " << driverDisplay.get_DisplayStatus_text() << "." << endl;
  }

  systemOk = true;
  sleep(1);
  carState.init_values();
#if IOEXT_ON || IOEXT2_ON
  ioExt.readAll();
#endif
  cout << "-----------------------------------------------------------------" << endl;
  cout << "Creating FreeRTOS tasks successful. System running." << endl;
  cout << "-----------------------------------------------------------------" << endl;
  cout << endl;
  cout << carState.print("Initial car state:") << endl;
}
