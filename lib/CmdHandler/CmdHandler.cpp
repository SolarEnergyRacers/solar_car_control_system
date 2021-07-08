//
// Command Receiver and Handler
//
// reads commands from serial console and deploy it
#include <definitions.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Arduino.h>
#include <Wire.h> // I2C
#include <inttypes.h>
#include <stdio.h>

#include "CmdHandler.h"
#include "DAC.h"
#include "DriverDisplayC.h"
#include "Helper.h"
#include "Indicator.h"

extern DAC dac;
extern Indicator indicator;

void CmdHandler::re_init() {
    init();
}

void CmdHandler::init() {
  // nothing to do, i2c bus is getting initialized externally
  printf("[v] Command handler inited\n");
}

void CmdHandler::exit() {
    // TODO
}

void CmdHandler::task() {

  DriverDisplayC *dd = DriverDisplayC::instance();
  while (1) {

    while (Serial.available() > 0) {
      // read the incoming chars:
      String input = Serial.readString();

#if DEBUGGINGLEVEL_VERBOSED == true // TODO: we could add a global debug level macro (i.e. depending on verbosity a number and activate/deactive printf statements at compile-time)
      printf("Received: %s\n", input.c_str());
#endif
      Serial.flush();
      if (input.length() < 1 || commands.lastIndexOf(input[0]) == -1) {
        input = "help";
      }

      if (input.endsWith("\n")) {
        input = input.substring(0, input.length() - 1);
      }
      if (input.endsWith("\r")) {
        input = input.substring(0, input.length() - 1);
      }

      switch (input[0]) {
      // ---------------- controller commands
      case 'R':
        dd->re_init();
        break;
      case 's':
        if (input[2] == 'f') {
          dd->write_drive_direction(DRIVE_DIRECTION::FORWARD);
        } else if (input[2] == 'b') {
          dd->write_drive_direction(DRIVE_DIRECTION::BACKWARD);
        } else {
          dd->write_speed(atoi(&input[1]));
        }
        break;
      case 'b':
        dd->write_bat(atof(&input[1]));
        break;
      case 'p':
        dd->write_pv(atof(&input[1]));
        break;
      case 'M':
        dd->write_motor(atof(&input[1]));
        break;
      case 'm':
        dd->write_speed(atoi(&input[1]));
        dac.set_pot(atoi(&input[1]), DAC::POT_CHAN0);
        break;
      // -------------- chase car commands
      case 'u':
        if (String("off") == String(&input[2])) {
          printf("%s:%s-->off\n", input.c_str(), &input[2]);
          dd->arrow_increase(false);
        } else {
          printf("%s:%s-->on\n", input.c_str(), &input[2]);
          dd->arrow_increase(true);
        }
        break;
      case 'd':
        if (String("off") == String(&input[2])) {
          printf("%s:%s-->off\n", input.c_str(), &input[2]);
          dd->arrow_decrease(false);
        } else {
          printf("%s:%s-->off\n", input.c_str(), &input[2]);
          dd->arrow_decrease(true);
        }
        break;
      case ':':
        dd->write_driver_info(&input[1], INFO_TYPE::INFO);
        break;
      case '!':
        dd->write_driver_info(&input[1], INFO_TYPE::WARN);
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
      case 'a':
        dd->write_acceleration(atoi(&input[1]));
        break;
      case 'l':
        dd->light1OnOff();
        break;
      case 'L':
        dd->light2OnOff();
        break;
      case 'c':
        if (input[2] == 's') {
          dd->write_constant_mode(CONSTANT_MODE::SPEED);
        } else {
          dd->write_constant_mode(CONSTANT_MODE::POWER);
        }
        break;
      // usage
      case 'h':
      default:
        printf("Unknown command '%s'\n", input.c_str());
        printf("%s", helpText.c_str());
      }
    }
    // sleep for some seconds
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
