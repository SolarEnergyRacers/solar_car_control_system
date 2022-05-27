/*
 *  SER - Solar Car Control System - Main Routine for setting up tasks,
 * initialize devices, ..
 *
 * clang style:
 *    ./extras/format.sh
 */

#include <fmt/core.h>

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

// Global objects (not possible to deactivate)
CarControl carControl;
CarState carState;
CmdHandler cmdHandler;
Console console;
DriverDisplay driverDisplay;
EngineerDisplay engineerDisplay;
GPInputOutput gpio; // I2C Interrupts, GPIO pin settings
I2CBus i2cBus;
Indicator indicator;
OneWireBus oneWireBus;
SDCard sdCard;
SPIBus spiBus;
// Global objects (possibly to deactivated)
#if INT_ON
TODO !!!GPInputOutput gpio; // I2C Interrupts, GPIO pin settings
#endif
#if IOEXT_ON
IOExt2 ioExt;
#endif
#if ADC_ON
ADC adc;
#endif
#if DS_ON
Temp ds; // temperature
#endif
#if GYRO_ACC_ON
GyroAcc gyroAcc;
#endif
#if PWM_ON
PWM pwm;
#endif
#if RTC_ON
RTC rtc;
#endif
#if DAC_ON
DAC dac;
#endif
#if CAN_ON
CANBus can; // TODO: gets a linking-error if we set CAN_ON to true
#endif
#if CARSPEED_ON
CarSpeed carSpeed;
#endif
#if RTC_ON
ESP32Time esp32time;
#endif

bool startOk = true;
bool systemOk = false;

bool debug = true;
bool debugl2 = false;
bool debugl3 = false;
// int debug_level = 3;

void app_main(void) {

#if SERIAL_RADIO_ON
  // init console IO and radio console
  Uart uart; // SERIAL
  uart.init();
  delay(300);
#endif

#if TEST_SERIAL2
  // Testcode for buffered Serial2 transfer
  console << "a:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8f9x1x\n";
  console << "b:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8f9x1\n";
  console << "c:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8f9x\n";
  console << "d:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8f9\n";
  console << "e:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8f\n";
  console << "f:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7f8\n";
  console << "g:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f7\n";
  console << "h:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6f\n";
  console << "i:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f6\n";
  console << "j:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5f\n";
  console << "k:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f5\n";
  console << "l:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4f\n";
  console << "m:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f4\n";
  console << "n:a1a2a3a4a5a6a7a8a9b1b2b3b4b5b6b7b8b9c1c2c3c4c5vc6c7c8c9d1d2d3d4d5d6d7d8d9e1e2e3e4e5e6e7e8e9f1f2f3f\n";
  console << "o:a1\n";
  console << "\n";
  console << "p:a1a2\n";
#endif

  console << "\n--------------------\n";
  console << "esp32dev + free RTOS\n";
  console << "Solar Energy Car Races SER4" << VERSION;
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
  // engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_CONSOLE);
  string msg = fmt::format("[v] {} task initialized ({}ms) {}.\n", engineerDisplay.getName(), engineerDisplay.get_SleepTime(),
                           engineerDisplay.get_DisplayStatus_text());
  console << msg << "\n";
  // engineerDisplay.print(msg);
// ---- init modules ----
#if INT_ON
  TODO !!!GPInputOutput gpio; // I2C Interrupts, GPIO pin settings
  gpio.register_gpio_interrupt();
#endif
#if IOEXT_ON
  ioExt.init();
#endif
#if SD_ON
  sdCard.init();
#endif

  //--- SD card available ----------------------------
  console << "carState.init_values():2\n";
  carState.init_values();
  //------from now config ini values can be used------

#if COMMANDHANDLER_ON
  cmdHandler.init();
#endif
#if RTC_ON
  rtc.init();
#endif
#if INDICATOR_ON
  indicator.init();
#endif
#if DAC_ON
  dac.init();
#endif
#if ADC_ON
  adc.init();
#endif
#if DS_ON
  ds.init();
#endif
#if GYRO_ACC_ON
  gyroAcc.init();
#endif
#if PWM_ON
  pwm.init();
#endif
#if CAN_ON
  can.init();
#endif
#if CARSPEED_ON
  carSpeed.init();
#endif

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
#if INDICATOR_ON
  indicator.create_task();
  engineerDisplay.print(fmt::format("[v] {} task initialized ({}ms).\n", indicator.getName(), indicator.get_SleepTime()));
#endif
#if DS_ON
  xTaskCreate(&read_ds_demo_task, "read_ds_demo_task", CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE, NULL, 5, NULL);
#endif
#if ADC_ON
  adc.create_task();
  engineerDisplay.print(fmt::format("[v] {} task initialized ({}ms).\n", adc.getName(), adc.get_SleepTime()));
#endif
#if GYRO_ACC_ON
  gyroAcc.create_task();
  engineerDisplay.print(fmt::format("[v] {} task initialized ({}ms).\n", gyroAcc.getName(), gyroAcc.get_SleepTime()));
#endif
#if PWM_ON
  pwm.create_task();
  engineerDisplay.print(fmt::format("[v] {} task initialized ({}ms).\n", pwm.getName(), pwm.get_SleepTime()));
#endif
#if RTC_ON
  RtcDateTime now = rtc.read_rtc_datetime();
  // console << fmt::sprintf("[RTC] current datetime: %02u/%02u/%04u %02u:%02u:%02u\n"
  //                , now.Month(), now.Day(), now.Year(), now.Hour(), now.Minute(), now.Second());
  esp32time.setTime(now.Second(), now.Minute(), now.Hour(), now.Day(), now.Month(), now.Year());
  // engineerDisplay.print("[v] " + rtc.getName() + " initialized, time in esp32 updated.\n");
#endif
#if INT_ON
  gpio.create_task();
  engineerDisplay.print(fmt::format("[v] {} task initialized ({}ms).\n", gpio.getName(), gpio.get_SleepTime()));
#endif
#if COMMANDHANDLER_ON
  cmdHandler.create_task();
  engineerDisplay.print(fmt::format("[v] {} task initialized ({}ms).\n", cmdHandler.getName(), cmdHandler.get_SleepTime()));
#endif
#if CAN_ON
  can.create_task();
  engineerDisplay.print(fmt::format("[v] {} task initialized ({}ms).\n", can.getName(), can.get_SleepTime()));
#endif
#if CARCONTROL_ON
  carControl.init();
  carControl.create_task();
  engineerDisplay.print(fmt::format("[v] {} task initialized ({}ms).\n", carControl.getName(), carControl.get_SleepTime()));
#endif
#if IOEXT_ON
  carState.Indicator = INDICATOR::OFF;
  carState.ConstantModeOn = false; // #SAFETY#: deceleration unlock const mode
  carState.SdCardDetect = false;
  carState.ConstantMode = CONSTANT_MODE::SPEED;
  carState.Light = LIGHT::OFF;
  ioExt.create_task(1, 250, 8192);
  msg = fmt::format("[v] {} task initialized ({}ms).\n", ioExt.getName(), ioExt.get_SleepTime());
  console << msg << "\n";
  engineerDisplay.print(msg);
  ioExt.readAll();
  engineerDisplay.print("[v] " + ioExt.getName() + " readAll done..\n");
#endif
#if CARSPEED_ON
  carSpeed.create_task();
  engineerDisplay.print(fmt::format("[v] {} task initialized ({}ms).\n", carSpeed.getName(), carSpeed.get_SleepTime()));
#endif
  //--let the bootscreen visible for a moment ------------------
  int waitAtConsoleView = 5;
  engineerDisplay.print("\nready");
#if RTC_ON == true
  engineerDisplay.print(fmt::format(" at {}", esp32time.getDateTime().c_str()));
#endif
  engineerDisplay.print(".\nWaiting for start of life display: ");
  while (waitAtConsoleView-- > 0) {
    engineerDisplay.print(to_string(waitAtConsoleView));
    sleep(1);
    engineerDisplay.print("-");
  }
  engineerDisplay.print("start");
  engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_HALTED);
//------------------------------------------------------------
#if ENGINEER_DISPLAY_ON
  engineerDisplay.create_task(10);
#endif
#if DRIVER_DISPLAY_ON
  driverDisplay.init();
  driverDisplay.set_DisplayStatus(DISPLAY_STATUS::DRIVER_SETUP);
  driverDisplay.create_task(16);
  console << "[v] " << driverDisplay.getName() << " task initialized, " << driverDisplay.get_DisplayStatus_text() << "\n";
#endif

  systemOk = true;

  // #if IOEXT_ON
  //   ioExt.readAll();
  // #endif
  console << "-----------------------------------------------------------------\n";
  console << "FreeRTOS tasks successfully created. System running.\n";
  console << "-----------------------------------------------------------------\n";

  // //--- SD card available ----------------------------
  // console << "carState.init_values():3\n";
  // carState.init_values();
  // //------from now config ini values can be used------
  console << "-----------------------------------------------------------------\n";
}
