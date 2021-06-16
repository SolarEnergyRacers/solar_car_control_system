//
// Command Receiver and Handler
//
// reads commands from serial console and deploy it
#include "../../include/definitions.h"
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

void init_command_handler() {
  // nothing to do, i2c bus is getting initialized externally
  printf("[v] Command handler inited\n");
}

String commands = "<>lLwudsabpmM:!";
String helpText = "Available commands (" + commands +
                  "):\n"
                  "\tu [off]  - speed up arrow (green)\n"
                  "\td [off]  - speed down arrow (red)\n"
                  "\ts ddd    - speed value [0...999]\n"
                  "\ta dd     - acceleration value [0...9]\n"
                  "\tb fff.f  - battary voltage [0...999]\n"
                  "\tp ffff.f - photovoltaics current [-999...+999]\n"
                  "\tm ffff.f - motor current [-999...+999]\n"
                  "\tM ddd    - set motor potentiometer [0...255]\n"
                  "\t:<text>  - display driver info text\n"
                  "\t!<text>  - display driver warn text\n"
                  "\t< [off]  - left indicator\n"
                  "\t> [off]  - right indicator\n"
                  "\tw [off]  - hazard warning lights\n"
                  "\tl [off]  - position lights on/off\n"
                  "\tL [off]  - beam light on/off\n"
                  "\t\n";

void command_handler_task(void *pvParameter) {

  DriverDisplayC *dd = DriverDisplayC::instance();
  while (1) {

    while (Serial.available() > 0) {
      // read the incoming chars:
      String input = Serial.readString();

#if DEBUGGINGLEVEL_VERBOSED == true
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
      case 's':
        dd->write_speed(atoi(&input[1]));
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
        set_pot(atoi(&input[1]), POT_CHAN0);
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
        dd->write_driver_info(&input[1], DriverDisplayC::INFO_TYPE::INFO);
        break;
      case '!':
        dd->write_driver_info(&input[1], DriverDisplayC::INFO_TYPE::WARN);
        break;
      // -------------- steering wheel input element emulators
      case '<':
        setIndicator(DriverDisplayC::INDICATOR::LEFT);
        break;
      case '>':
        setIndicator(DriverDisplayC::INDICATOR::RIGHT);
        break;
      case 'w':
        setIndicator(DriverDisplayC::INDICATOR::WARN);
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
