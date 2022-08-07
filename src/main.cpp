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
#include <IOExt.h>
#include <IOExtHandler.h>
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
GPInputOutput gpio; // I2C Interrupts, GPInputOutput pin settings
I2CBus i2cBus;
Indicator indicator;
OneWireBus oneWireBus;
SDCard sdCard;
SPIBus spiBus;
// Global objects (possibly to deactivated)
IOExt ioExt;
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
ESP32Time esp32time;
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

bool startOk = true;
bool systemOk = false;

bool debug = true;
bool debugl2 = false;
bool debugl3 = false;
// int debug_level = 3;

string msg;

void app_main(void) {

  console << "\n";

#if SERIAL_RADIO_ON
  // init console IO and radio console
  Uart uart; // SERIAL
  msg = uart.init();
  console << msg << "\n";
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
  console << "Solar Energy Car Racers SER4 Controller: " << VERSION;
  console << "\n--------------------\n";

  // init arduino library
  initArduino();

  // report chip info
  console << "-chip info -------------------\n";
  chip_info();
  console << "-gpio pin settings ----------\n";
  msg = gpio.init();
  delay(2000);
  console << msg << "\n";
  console << "-init bus systems ------------\n";
  // init buses
  msg = spiBus.init();
  console << msg << "\n";
  msg = oneWireBus.init();
  console << msg << "\n";
  msg = i2cBus.init();
  console << msg << "\n";

  msg = engineerDisplay.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
// ---- init modules ----
#if INT_ON
  gpio.register_gpio_interrupt();
#endif

  // IOEXT
  msg = ioExt.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
  // console << "     Reread all IOs in foreced mode:";
  // ioExt.readAndHandlePins(PinHandleMode::FORCED);
  // console << carState.printIOs("", true, false) << "\n";

#if SD_ON
  sdCardDetectHandler();
  msg = sdCard.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
  console << "---01------------------------------------------\n";
#endif

  //--- SD card available ----------------------------
  carState.init_values();
  sdCard.open_log_file();
  //------from now config ini values can be used------

#if COMMANDHANDLER_ON
  msg = cmdHandler.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if RTC_ON
  msg = rtc.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if INDICATOR_ON
  msg = indicator.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if DAC_ON
  msg = dac.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if ADC_ON
  msg = adc.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if DS_ON
  msg = ds.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if GYRO_ACC_ON
  msg = gyroAcc.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if PWM_ON
  msg = pwm.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if CAN_ON
  msg = can.init();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
  // #if CARCONTROL_ON
  //   carControl.init();
  //   console << msg << "\n";
  //   engineerDisplay.print(msg + "\n");
  // #endif
  // #if CARSPEED_ON
  //   msg = carSpeed.init();
  //   console << msg << "\n";
  //   engineerDisplay.print(msg + "\n");
  // #endif

  if (!startOk) {
    console << "ERROR in init sequence(s). System halted!\n";
    exit(0);
  }

  engineerDisplay.print("Startup sequence successful, creating FreeRTOS tasks.\n");
  console << "\n";
  console << "-----------------------------------------------------------------\n";
  console << "Startup sequence(s) successful. System creating FreeRTOS tasks...\n";
  console << "-----------------------------------------------------------------\n\n";
  // ---- create tasks ----

  // IOEXT
  carState.Indicator = INDICATOR::OFF;
  carState.ConstantModeOn = false; // #SAFETY#: deceleration unlock const mode
  carState.SdCardDetect = false;
  carState.ConstantMode = CONSTANT_MODE::SPEED;
  carState.Light = LIGHT::OFF;
  msg = ioExt.create_task(4, 100, 6000);
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");

#if INDICATOR_ON
  msg = indicator.create_task();
  engineerDisplay.print(msg + "\n");
  console << msg << "\n";
#endif
#if DS_ON
  msg = ds.create_task();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if ADC_ON
  msg = adc.create_task();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if GYRO_ACC_ON
  msg = gyroAcc.create_task();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if PWM_ON
  msg = pwm.create_task();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if RTC_ON
  RtcDateTime now = rtc.read_rtc_datetime();
  esp32time.setTime(now.Second(), now.Minute(), now.Hour(), now.Day(), now.Month(), now.Year());
  string actTime = formatDateTime(now);
  msg = rtc.create_task();
  console << msg << " RTC Time: " << actTime << "\n";
  engineerDisplay.print(fmt::format("{} {}\n", msg, actTime));
#endif
#if INT_ON
  msg = gpio.create_task();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if COMMANDHANDLER_ON
  msg = cmdHandler.create_task(20, 350, 6000);
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if CAN_ON
  msg = can.create_task();
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if CARCONTROL_ON
  msg = carControl.init();
  console << msg << "\n";
  msg = carControl.create_task(10, 250, 6000);
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif
#if CARSPEED_ON
  msg = carSpeed.init();
  console << msg << "\n";
  msg = carSpeed.create_task(10, 250, 3000);
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
#endif

  engineerDisplay.print("\nready");
#if RTC_ON
  engineerDisplay.print(fmt::format(" at {}", esp32time.getDateTime().c_str()));
#endif
  //--let the bootscreen visible for a moment ------------------
  engineerDisplay.print(".\nWaiting for start of life display: ");
  int waitAtConsoleView = 5;
  while (waitAtConsoleView-- > 0) {
    engineerDisplay.print(to_string(waitAtConsoleView));
    delay(1000);
    engineerDisplay.print("-");
  }
  engineerDisplay.print("start");
  engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_HALTED);
  //------------------------------------------------------------
  msg = engineerDisplay.create_task(10);
  console << msg << "\n";
  engineerDisplay.print(msg + "\n");
  driverDisplay.init();
  driverDisplay.set_DisplayStatus(DISPLAY_STATUS::DRIVER_SETUP);
  msg = driverDisplay.create_task(16);
  console << msg << driverDisplay.get_DisplayStatus_text() << "\n";
  engineerDisplay.print(msg + "\n");
  delay(1000);

  systemOk = true;

  console << "-----------------------------------------------------------------\n";
  console << "FreeRTOS tasks successfully created. System running.\n";
  console << "-----------------------------------------------------------------\n";

  ioExt.readAndHandlePins(PinHandleMode::FORCED);
}
