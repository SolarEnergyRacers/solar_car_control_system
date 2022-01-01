//
// Command Receiver and Handler
//
// reads commands from serial console and deploy it
#include <definitions.h>

#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Arduino.h>
#include <I2CBus.h>
#include <Wire.h> // I2C

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
#include <IOExt2.h>
#include <Indicator.h>
#include <SDCard.h>
#include <system.h>

extern I2CBus i2cBus;
extern DAC dac;
extern ADC adc;
extern IOExt2 ioExt;
extern I2CBus i2cBus;
extern Indicator indicator;
extern CarState carState;
extern CarControl carControl;
extern DriverDisplay driverDisplay;
extern EngineerDisplay engineerDisplay;
extern SDCard sdCard;

using namespace std;

// ------------------
// FreeRTOS functions

void CmdHandler::re_init() { init(); }

void CmdHandler::init() {
  // nothing to do, i2c bus is getting initialized externally
  string s = "[v] " + getName() + " initialized.\n";
  cout << s;
  driverDisplay.print(s.c_str());
}

void CmdHandler::exit() {
  // TODO
}
// ------------------

void CmdHandler::task() {
  string state;
  while (1) {
    try {
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
        case 's':
          cout << carState.print("Recent State") << endl;
          printSystemValues();
          break;
        case 'S':
          cout << carState.print("Recent State:") << endl;
          break;
        case 'J':
          state = carState.serialize("Recent State");
          cout << state << endl;
          sdCard.write(state + "\n");
          break;
        case 'V':
          state = carState.csv("Recent State");
          cout << state;
          sdCard.write(state);
          break;
        case 'P': {
          sdCard.directory();
        } break;
        case 'U':
          sdCard.unmount();
          break;
        case 'M':
          sdCard.logEnabled = sdCard.mount();
          break;
        case 'H':
          memory_info();
          break;
        // -------------- chase car commands
        case '-':
          carControl.adjust_paddles(3); // manually adjust paddles (3s handling time)
          break;
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
          cout << "ERROR:: Unknown command '" << input << "' " << endl << helpText << endl;
          break;
        case 'h':
          cout << helpText << endl;
          break;
        }
      }
    } catch (exception &ex) {
      cout << "Exception:" << ex.what() << endl;
    }
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}

void CmdHandler::printSystemValues() {

  int16_t valueRec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);
  string s = fmt::format("v0={:5d}  v1={:5d}\n", valueRec, valueAcc);
  cout << s;

  for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < MCP23017_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->value == 0) {
        s = fmt::format("{}: SET {:#04x}", pin->name, pin->port);
        cout << s << endl;
      }
    }
  }
  s = fmt::format("POT-0 (accel)= {:4d}, POT-1 (recup)= {:4d}", dac.get_pot(DAC::pot_chan::POT_CHAN0),
                  dac.get_pot(DAC::pot_chan::POT_CHAN1));
  cout << s << endl;
}
