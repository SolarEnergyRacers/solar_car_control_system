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
#include <stdio.h>

#include <ADC.h>
#include <CarState.h>
#include <CarStatePin.h>
#include <CarStateValue.h>
#include <CmdHandler.h>
#include <DAC.h>
#include <Display.h>
#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
#include <IOExt.h>
#include <Indicator.h>
#include <sstream>
#include <string>

extern I2CBus i2cBus;
extern DAC dac;
extern ADC adc;
extern IOExt ioExt;
extern Indicator indicator;
extern CarState carState;
extern DriverDisplay driverDisplay;
extern EngineerDisplay engineerDisplay;

void CmdHandler::re_init() { init(); }

void CmdHandler::init() {
  // nothing to do, i2c bus is getting initialized externally
  printf("[v] Command handler inited\n");
  driverDisplay.print("[v] " + getName() + " initialized.\n");
}

void CmdHandler::exit() {
  // TODO
}

void CmdHandler::task() {

  while (1) {

    while (Serial.available() > 0) {
      int value = 0;
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

      int accValue;
      switch (input[0]) {
      // ---------------- controller commands
      case 'R':
        engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::HALTED);
        driverDisplay.re_init();
        driverDisplay.set_DisplayStatus(DISPLAY_STATUS::SETUPDRIVER);
        break;
      case 'D':
        engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::HALTED);
        driverDisplay.set_DisplayStatus(DISPLAY_STATUS::SETUPDRIVER);
        break;
      case 'C':
        driverDisplay.set_DisplayStatus(DISPLAY_STATUS::HALTED);
        engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::HALTED);
        driverDisplay.set_DisplayStatus(DISPLAY_STATUS::CONSOLE);
        driverDisplay.clear_screen(ILI9341_WHITE);
        break;
      case 'E':
        driverDisplay.set_DisplayStatus(DISPLAY_STATUS::HALTED);
        engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::SETUPENGINEER);
        break;
      case 'S':
        printSystemValues();
        debug_printf("%s\n", carState.print("Recent State").c_str());
        if (input[1] == 'J') {
          debug_printf("%s\n", carState.serialize("Recent State").c_str());
        }
        break;
      case '-':
        adc.adjust_min_acc_dec();
        break;
      case '=':
        adc.adjust_max_acc_dec();
        break;
      case 's':
        if (input[2] == 'f') {
          carState.DriveDirection.set(DRIVE_DIRECTION::FORWARD);
          driverDisplay.write_drive_direction(DRIVE_DIRECTION::FORWARD);
        } else if (input[2] == 'b') {
          carState.DriveDirection.set(DRIVE_DIRECTION::BACKWARD);
          driverDisplay.write_drive_direction(DRIVE_DIRECTION::BACKWARD);
        } else {
          carState.Speed.set(atoi(&input[1]));
          driverDisplay.write_speed();
        }
        break;
      case 'b':
        driverDisplay.write_bat(atof(&input[1]));
        break;
      case 'p':
        driverDisplay.write_pv(atof(&input[1]));
        break;
      case 'm':
        driverDisplay.write_motor(atof(&input[1]));
        break;
      case 'a':
        accValue = atoi(&input[1]);
        driverDisplay.write_acceleration(accValue);
        // TODO: where to put in this important
        if (accValue > 0) {
          dac.set_pot(accValue, DAC::POT_CHAN0);
          dac.set_pot(0, DAC::POT_CHAN1);
        } else if (accValue > 0) {
          dac.set_pot(0, DAC::POT_CHAN0);
          dac.set_pot(accValue, DAC::POT_CHAN1);
        } else {
          dac.set_pot(0, DAC::POT_CHAN0);
          dac.set_pot(0, DAC::POT_CHAN1);
        }
        break;
      case 'A':
        value = atoi(&input[1]);
        if (value > 0) {
          dac.set_pot(value, DAC::POT_CHAN0);
          dac.set_pot(0, DAC::POT_CHAN1);
        } else if (value < 0) {
          dac.set_pot(0, DAC::POT_CHAN0);
          dac.set_pot(value, DAC::POT_CHAN1);
        } else {
          dac.set_pot(0, DAC::POT_CHAN0);
          dac.set_pot(0, DAC::POT_CHAN1);
        }
        break;

      // -------------- chase car commands
      case 'u':
        if (string("off") == string(&input[2])) {
          debug_printf("%s:%s-->off\n", input.c_str(), &input[2]);
          driverDisplay.arrow_increase(false);
        } else {
          debug_printf("%s:%s-->on\n", input.c_str(), &input[2]);
          driverDisplay.arrow_increase(true);
        }
        break;
      case 'd':
        if (string("off") == string(&input[2])) {
          debug_printf("%s:%s-->off\n", input.c_str(), &input[2]);
          driverDisplay.arrow_decrease(false);
        } else {
          debug_printf("%s:%s-->on\n", input.c_str(), &input[2]);
          driverDisplay.arrow_decrease(true);
        }
        break;
      case ':':
        driverDisplay.write_driver_info(&input[1], INFO_TYPE::INFO);
        break;
      case '!':
        driverDisplay.write_driver_info(&input[1], INFO_TYPE::WARN);
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
          carState.Light.set(LIGHT::OFF);
        } else {
          carState.Light.set(LIGHT::L1);
        }
        driverDisplay.show_light();
        break;
      case 'L':
        if (input[1] == '-') {
          carState.Light.set(LIGHT::OFF);
        } else {
          carState.Light.set(LIGHT::L2);
        }
        driverDisplay.show_light();
        break;
      case 'c':
        if (input[2] == 's') {
          carState.ConstantMode.set(CONSTANT_MODE::SPEED);
        } else if (input[2] == 'p') {
          carState.ConstantMode.set(CONSTANT_MODE::POWER);
        } else if (input[2] == 'c') {
          carState.ConstantMode.set(CONSTANT_MODE::NONE);
        } else {
          carState.ConstantModeOn.set(false);
        }
        driverDisplay.constant_drive_mode_show();
        break;
      case 'i':
        ioExt.readAll();
        break;
      // usage
      default:
        printf("ERROR:: Unknown command '%s'\n%s\n", input.c_str(), helpText.c_str());
        break;
      case 'h':
        printf("%s", helpText.c_str());
        break;
      }
    }
    // sleep for sleep_polling_ms
    this->sleep(200);
  }
}

void CmdHandler::printSystemValues() {

  int16_t valueRec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);
  printf("v0: %5d\tv1: %5d\n", valueRec, valueAcc);
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->value == 0) {
        printf("%s: SET 0x%02x\n", pin->name.c_str(), pin->port);
      }
    }
  }
  printf("POT0 (accel)= %4d, POT1 (recup)= %4d\n", dac.get_pot(DAC::pot_chan::POT_CHAN0), dac.get_pot(DAC::pot_chan::POT_CHAN1));
}
