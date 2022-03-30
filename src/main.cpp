/*
 *  SER - Solar Car Control System - Main Routine for setting up tasks,
 * initialize devices, ..
 *
 * clang style:
 *    ./extras/format.sh
 */

#include <fmt/core.h>
#include <fmt/printf.h>

// local definitions
#include <definitions.h>

// standard libraries
#include <Streaming.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// FreeRTOS / Arduino
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <freertos/FreeRTOS.h>
#include <freertos/FreeRTOSConfig.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

// project variables
#include <sdkconfig.h>

// local libs
#include <ADC.h>
#include <CANBus.h>
#include <CarSpeed.h>
#include <CmdHandler.h>
#include <Console.h>
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
CANBus can; // TODO: gets a linking-error if we set CAN_ON to true
OneWireBus oneWireBus;
Console console;
SPIBus spiBus;
I2CBus i2cBus;
Temp ds; // temperature
SDCard sdCard;
CmdHandler cmdHandler;
DAC dac;
DriverDisplay driverDisplay;
EngineerDisplay engineerDisplay;
ESP32Time esp32time;
GPInputOutput gpio; // I2C Interrupts, GPIO pin settings
GyroAcc gyroAcc;
Indicator indicator; // INDICATOR_ON
IOExt2 ioExt;
PWM pwm;
RTC rtc;
Uart uart; // SERIAL

CarSpeed carSpeed;
CarControl carControl;
CarState carState;

bool startOk = true;
bool systemOk = false;

void app_main(void) {

  if (SERIAL_RADIO_ON) {
    // init console IO and radio console
    uart.init();
    delay(300);
  }

  // Testcode for buffered Serial2 tranfer
  // console << "a:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8f9x1x\n";
  // console << "b:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8f9x1\n";
  // console << "c:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8f9x\n";
  // console << "d:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8f9\n";
  // console << "e:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8f\n";
  // console << "f:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8\n";
  // console << "g:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7\n";
  // console << "h:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f\n";
  // console << "i:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6\n";
  // console << "j:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f\n";
  // console << "k:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5\n";
  // console << "l:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f\n";
  // console << "m:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4\n";
  // console << "n:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f\n";
  // console << "o:a1\n";
  // console << "\n";
  // console << "o:a1a2\n";

  console << "\n--------------------\n";
  console << "esp32dev + free RTOS\n";
  console << "Solar Energy Car Races SER4" << VERSION << " -- " << VERSION_PUBLISHED;
  console << "\n--------------------\n";

  // init arduino library
  initArduino();

  // report chip info
  console << "-chip info -------------------\n";
  chip_info();
  console << "-gpio pin settings ----------\n";
  gpio.init();
  console << "-init bus systems ------------\n";
  // init buses
  spiBus.init();
  oneWireBus.init();
  i2cBus.init();
  console << "------------------------------\n";

  engineerDisplay.init();
  engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_CONSOLE);
  engineerDisplay.print("[v] " + engineerDisplay.getName() + " initialized, " + engineerDisplay.get_DisplayStatus_text() + ".\n");

  delay(1000);

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
  if (INT_ON) {
    gpio.register_gpio_interrupt();
  }
  if (IOEXT2_ON) {
    ioExt.init();
  }
  if (DAC_ON) {
    dac.init();
  }
  if (CAN_ON) {
    can.init();
  }
  if (SD_ON) {
    sdCard.init();
  }
  if (CARSPEED_ON) {
    carSpeed.init();
  }
  if (!startOk) {
    console << "ERROR in init sequence(s). System halted!\n";
    exit(0);
  }

  engineerDisplay.print("Startup sequence(s) successful.\n");
  engineerDisplay.print("System creating FreeRTOS tasks...\n");
  console << "\n";
  console << "-----------------------------------------------------------------\n";
  console << "Startup sequence(s) successful. System creating FreeRTOS tasks...\n";
  console << "-----------------------------------------------------------------\n\n";

  // ---- create tasks ----
  if (INDICATOR_ON) {
    indicator.create_task();
    engineerDisplay.print("[v] " + indicator.getName() + " task initialized.\n");
  }
  if (DS_ON) {
    xTaskCreate(&read_ds_demo_task, "read_ds_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
  }
  if (GYRO_ACC_ON) {
    // xTaskCreate(&read_gyro_acc_demo_task, "read_gyro_acc_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
    gyroAcc.create_task();
    engineerDisplay.print("[v] " + gyroAcc.getName() + " task initialized.\n");
  }
  if (PWM_ON) {
    pwm.create_task();
    engineerDisplay.print("[v] " + pwm.getName() + " task initialized.\n");
  }
  if (RTC_ON) {
    RtcDateTime now = rtc.read_rtc_datetime();
    // console << fmt::sprintf("[RTC] current datetime: %02u/%02u/%04u %02u:%02u:%02u\n"
    //                , now.Month(), now.Day(), now.Year(), now.Hour(), now.Minute(), now.Second());
    esp32time.setTime(now.Second(), now.Minute(), now.Hour(), now.Day(), now.Month(), now.Year());
    engineerDisplay.print("[v] " + rtc.getName() + " initialized, time in esp32 updated.\n");
  }
  if (INT_ON) {
    gpio.create_task();
    engineerDisplay.print("[v] " + gpio.getName() + " task initialized.\n");
  }
  if (DAC_ON) {
    dac.init();
  }
  if (COMMANDHANDLER_ON) {
    cmdHandler.create_task();
    engineerDisplay.print("[v] " + cmdHandler.getName() + " task initialized.\n");
  }
  // if (SERIAL_RADIO_ON) {
  //   console << " - serial_demo_task\n";
  //   uart.create_task();
  // }
  if (CAN_ON) {
    console << " - read_can_demo_task\n";
    can.create_task();
  }
  if (CARCONTROL_ON) {
    carControl.init();
    carControl.create_task();
    engineerDisplay.print("[v] " + carControl.getName() + " task initialized.\n");
  }
  if (IOEXT2_ON) {
    carState.Indicator = INDICATOR::OFF;
    carState.ConstantModeOn = false; //#SAVETY#: deceleration unlock const mode
    carState.SdCardDetect = false;
    carState.ConstantMode = CONSTANT_MODE::SPEED;
    carState.Light = LIGHT::OFF;
    ioExt.create_task();
    engineerDisplay.print("[v] " + ioExt.getName() + " task initialized.\n");
    ioExt.readAll();
  }
  if (CARSPEED_ON) {
    carSpeed.create_task();
    engineerDisplay.print("[v] " + carSpeed.getName() + " task initialized.\n");
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
    console << "[v] " << driverDisplay.getName() << " task initialized, " << driverDisplay.get_DisplayStatus_text() << "\n";
  }

  systemOk = true;
  sleep(1);
  carState.init_values();
#if IOEXT_ON || IOEXT2_ON
  ioExt.readAll();
#endif
  console << "-----------------------------------------------------------------\n";
  console << "Creating FreeRTOS tasks successful. System running.\n";
  console << "-----------------------------------------------------------------\n";
  console << "\n";
  console << carState.print("Initial car state:") << "\n";
  console << "-----------------------------------------------------------------\n";
}
