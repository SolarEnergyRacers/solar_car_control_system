//
// Command Receiver and Handler
//
// reads commands from serial console and deploy it
#include <definitions.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Arduino.h>
#include <I2CBus.h>
#include <Wire.h> // I2C
#include <inttypes.h>
#include <sstream>
#include <stdio.h>
#include <string>

#include <ADC.h>
#include <CarControl.h>
#include <CarState.h>
#include <CarStatePin.h>
#include <CmdHandler.h>
#include <DAC.h>
#include <Display.h>
#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
#if IOEXT_ON
#include <IOExt.h>
#endif
#if IOEXT2_ON
#include <IOExt2.h>
#endif
#include <Indicator.h>

extern I2CBus i2cBus;
extern DAC dac;
extern ADC adc;
#if IOEXT_ON
extern IOExt ioExt;
#endif
#if IOEXT2_ON
extern IOExt2 ioExt;
#endif
extern I2CBus i2cBus;
extern Indicator indicator;
extern CarState carState;
extern CarControl carControl;
extern DriverDisplay driverDisplay;
extern EngineerDisplay engineerDisplay;

// ------------------
// FreeRTOS functions

void CmdHandler::re_init() { init(); }

void CmdHandler::init() {
  // nothing to do, i2c bus is getting initialized externally
  printf("[v] Command handler inited\n");
  driverDisplay.print("[v] " + getName() + " initialized.\n");
}

void CmdHandler::exit() {
  // TODO
}
// ------------------

void CmdHandler::task() {

  while (1) {

    while (Serial.available() > 0) {
      // read the incoming chars:
      String input = Serial.readString();
      Serial.flush();

      if (input.endsWith("\n")) {
        input = input.substring(0, input.length() - 1);
      }
      if (input.endsWith("\r")) {
        input = input.substring(0, input.length() - 1);
      }

      debug_printf("Received: %s\n", input.c_str());

      if (input.length() < 1 || commands.find(input[0], 0) == -1) {
        input = "h"; // help
      }

      int intValue = 0;
      float floatValue = 0.0;
      if (input.length() > 1) {
        intValue = atoi(&input[1]);
        floatValue = atof(&input[1]);
      }
      switch (input[0]) {
      // ---------------- controller commands
      case 'R':
        engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_HALTED);
        driverDisplay.re_init();
        break;
      case 'C':
        driverDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_CONSOLE);
        engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_HALTED);
        driverDisplay.clear_screen(ILI9341_WHITE);
        break;
      case 'D':
        engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_HALTED);
        driverDisplay.set_DisplayStatus(DISPLAY_STATUS::DRIVER_SETUP);
        break;
      case 'E':
        driverDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_HALTED);
        engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::ENGINEER_SETUP);
        break;
      case 'S':
        printSystemValues();
        printf("%s\n", carState.print("Recent State").c_str());
        if (input[1] == 'J') {
          printf("%s\n", carState.serialize("Recent State").c_str());
        }
        break;
      case 's':
        if (input[2] == 'f') {
          carState.DriveDirection = DRIVE_DIRECTION::FORWARD;
        } else if (input[2] == 'b') {
          carState.DriveDirection = DRIVE_DIRECTION::BACKWARD;
        } else {
          carState.Speed = intValue;
        }
        break;
      case 'b':
        carState.BatteryVoltage = floatValue;
        break;
      case 'B':
        carState.BatteryCurrent = floatValue;
        break;
      case 'p':
        carState.PhotoVoltaicCurrent = floatValue;
        break;
      case 'm':
        carState.MotorCurrent = floatValue;
        break;
      case '-':
        carControl.adjust_paddles(10);
        break;
      case 'a':
        carState.Acceleration = intValue;
        if (intValue > 0) {
          dac.set_pot(intValue, DAC::POT_CHAN0);
          dac.set_pot(0, DAC::POT_CHAN1);
        } else if (intValue < 0) {
          dac.set_pot(0, DAC::POT_CHAN0);
          dac.set_pot(intValue, DAC::POT_CHAN1);
        } else {
          dac.set_pot(0, DAC::POT_CHAN0);
          dac.set_pot(0, DAC::POT_CHAN1);
        }
        break;

      // -------------- chase car commands
      case 'u':
        if (string("off") == string(&input[2])) {
          debug_printf("%s:%s-->off\n", input.c_str(), &input[2]);
          carState.SpeedArrow = SPEED_ARROW::OFF;
        } else {
          debug_printf("%s:%s-->on\n", input.c_str(), &input[2]);
          carState.SpeedArrow = SPEED_ARROW::INCREASE;
        }
        break;
      case 'd':
        if (string("off") == string(&input[2])) {
          debug_printf("%s:%s-->off\n", input.c_str(), &input[2]);
          carState.SpeedArrow = SPEED_ARROW::OFF;
        } else {
          debug_printf("%s:%s-->on\n", input.c_str(), &input[2]);
          carState.SpeedArrow = SPEED_ARROW::DECREASE;
        }
        break;
      case ':':
        carState.DriverInfoType = INFO_TYPE::INFO;
        carState.DriverInfo = &input[1];
        break;
      case '!':
        carState.DriverInfoType = INFO_TYPE::WARN;
        carState.DriverInfo = &input[1];
        break;
      // -------------- steering wheel input element emulators
      case '<':
        indicator.setIndicator(INDICATOR::LEFT);
        break;
      case '>':
        indicator.setIndicator(INDICATOR::RIGHT);
        break;
      case 'w':
        indicator.setIndicator(INDICATOR::WARN);
        break;
      case 'l':
        if (input[1] == '-') {
          carState.Light = LIGHT::OFF;
        } else {
          carState.Light = LIGHT::L1;
        }
        break;
      case 'L':
        if (input[1] == '-') {
          carState.Light = LIGHT::OFF;
        } else {
          carState.Light = LIGHT::L2;
        }
        break;
      case 'c':
        if (input[2] == 's') {
          carState.ConstantMode = CONSTANT_MODE::SPEED;
          carState.ConstantModeOn = true;
        } else if (input[2] == 'p') {
          carState.ConstantMode = CONSTANT_MODE::POWER;
          carState.ConstantModeOn = true;
        } else {
          if (carState.ConstantModeOn) {
            carState.ConstantModeOn = false;
          } else {
            carState.ConstantModeOn = false;
          }
        }
        break;
      case 'i':
        ioExt.readAll();
        break;
      case 'I':
        i2cBus.scan_i2c_devices();
        break;
      default:
        printf("ERROR:: Unknown command '%s'\n%s\n", input.c_str(), helpText.c_str());
        break;
      case 'h':
        printf("%s", helpText.c_str());
        break;
      }
    }
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}

void CmdHandler::printSystemValues() {

  int16_t valueRec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);
  printf("v0: %5d\tv1: %5d\n", valueRec, valueAcc);
#if IOEXT_ON
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->value == 0) {
        printf("%s: SET 0x%02x\n", pin->name.c_str(), pin->port);
      }
    }
  }
#endif
#if IOEXT2_ON
  for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < MCP23017_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->value == 0) {
        printf("%s: SET 0x%02x\n", pin->name.c_str(), pin->port);
      }
    }
  }
#endif
  printf("POT0 (accel)= %4d, POT1 (recup)= %4d\n", dac.get_pot(DAC::pot_chan::POT_CHAN0), dac.get_pot(DAC::pot_chan::POT_CHAN1));
}
