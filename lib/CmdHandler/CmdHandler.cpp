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
#include "DriverDisplay.h"
#include "Helper.h"
#include "Indicator.h"

void init_command_handler() {
  // nothing to do, i2c bus is getting initialized externally
  printf("Command handler task inited\n");
}

String commands = "<>lLwudsabpmM:";
String helpText = "Available commands (%s):\n"
                  "\t< [off]  - left indicator\n"
                  "\t> [off]  - right indicator\n"
                  "\tw [off]  - hazard warning lights\n"
                  "\tu [off]  - speed up arrow (green)\n"
                  "\td [off]  - speed down arrow (red)\n"
                  "\ts ddd    - speed value [0...999]\n"
                  "\ta dd     - acceleration value [0...9]\n"
                  "\tb fff.f  - battary voltage [0...999]\n"
                  "\tp ffff.f - photovoltaics current [-999...+999]\n"
                  "\tm ffff.f - motor current [-999...+999]\n"
                  "\tM ddd    - set motor potentiometer [0...255]\n"
                  "\tl [off]  - position lights on/off\n"
                  "\tL [off]  - beam light on/off\n"
                  "\t:<text>  - display driver info text\n"
                  "\t\n";

void command_handler_task(void *pvParameter) {

  //   int n = 30;
  //   char input_str[n];

  while (1) {

    // printf("command > ");
    while (Serial.available() > 0) {
      // read the incoming chars:
      String input = Serial.readString();
      // fgets_stdio_blocking(input_str, n);
      // scanf("%s", input_str);
      // printf("got command: '%s'\n", input_str);
      // String input = input_str;

      if (input.length() < 1 || commands.lastIndexOf(input[0]) == -1) {
        input = "help";
      }
      int16_t val = -1;
      switch (input[0]) {
      case '<':
        if (String("off") == String(&input[2])) {
          indicator_set_and_blink(INDICATOR::OFF);
        } else {
          indicator_set_and_blink(INDICATOR::LEFT);
        }
        break;
      case '>':
        if (String("off") == String(&input[2])) {
          indicator_set_and_blink(INDICATOR::OFF);
        } else {
          indicator_set_and_blink(INDICATOR::RIGHT);
        }
        break;
      case 'w':
        if (String("off") == String(&input[2])) {
          indicator_set_and_blink(INDICATOR::OFF);
        } else {
          indicator_set_and_blink(INDICATOR::WARN);
        }
        break;
      case 'u':
        if (String("off") == String(&input[2])) {
          arrow_increase(false);
        } else {
          arrow_increase(true);
        }
        break;
      case 'd':
        if (String("off") == String(&input[2])) {
          arrow_decrease(false);
        } else {
          arrow_decrease(true);
        }
        break;
      case 's':
        write_speed(atoi(&input[1]));
        break;
      case 'a':
        write_acceleration(atoi(&input[1]));
        break;
      case 'b':
        write_bat(atof(&input[1]));
        break;
      case 'p':
        write_pv(atof(&input[1]));
        break;
      case 'm':
        write_motor(atof(&input[1]));
        break;
      case 'M':
        val = atoi(&input[1]);
        printf("Motor Poti: %d\n", val);
        set_pot(val, POT_CHAN0);
        break;
      case 'l':
        light1OnOff();
        break;
      case 'L':
        light2OnOff();
        break;
      case ':':
        write_driver_info(&input[1], INFO_TYPE::INFO);
        break;
      case 'h':
      default:
        printf("Unknown command '%s'\n", input.c_str());
        printf(helpText.c_str(), commands);
      }
    }
    // sleep for 10s
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// void command_handler_task(void *pvParameter) {

//   while (1) {

//     while (Serial.available() > 0) {
//       digitalWrite(LED_BUILTIN, HIGH);
//       // read the incoming chars:
//       String input = Serial.readString();
//       printf("Received command: %s\n", input.c_str());

//       if (input.length() < 1 || commands.lastIndexOf(input[0]) == -1) {
//         input = "help";
//       }

//       switch (input[0]) {
//       case 'l':
//         if (String("off") == String(&input[2])) {
//           indicator_set_and_blink(INDICATOR::OFF);
//         } else {
//           indicator_set_and_blink(INDICATOR::LEFT);
//         }
//         break;
//       case 'r':
//         if (String("off") == String(&input[2])) {
//           indicator_set_and_blink(INDICATOR::OFF);
//         } else {
//           indicator_set_and_blink(INDICATOR::RIGHT);
//         }
//         break;
//       case 'w':
//         if (String("off") == String(&input[2])) {
//           indicator_set_and_blink(INDICATOR::OFF);
//         } else {
//           indicator_set_and_blink(INDICATOR::WARN);
//         }
//         break;
//       case 'u':
//         if (String("off") == String(&input[2])) {
//           arrow_increase(false);
//         } else {
//           arrow_increase(true);
//         }
//         break;
//       case 'd':
//         if (String("off") == String(&input[2])) {
//           arrow_decrease(false);
//         } else {
//           arrow_decrease(true);
//         }
//         break;
//       case 's':
//         write_speed(atoi(&input[1]));
//         break;
//       case 'a':
//         write_acceleration(atoi(&input[1]));
//         break;
//       case 'b':
//         write_bat(atof(&input[1]));
//         break;
//       case 'p':
//         write_pv(atof(&input[1]));
//         break;
//       case 'm':
//         write_motor(atof(&input[1]));
//         break;
//       case 'M':
//         set_pot(atoi(&input[1]), POT_CHAN0);
//         break;
//       case 'h':
//       default:
//         printf("Unknown command '%s'\n%s\n", input.c_str(),
//         helpText.c_str());
//       }
//     }
//   }

//   // sleep for 10s
//   vTaskDelay(100 / portTICK_PERIOD_MS);
// }
