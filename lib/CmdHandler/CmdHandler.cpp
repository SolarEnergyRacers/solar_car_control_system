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
#include <CmdHandler.h>
#include <DAC.h>
#include <DriverDisplayC.h>
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

void CmdHandler::re_init() { init(); }

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

      if (input.length() < 1 || commands.lastIndexOf(input[0]) == -1) {
        input = "h"; // help
      }

      int accValue;
      switch (input[0]) {
      // ---------------- controller commands
      case 'R':
        dd->re_init();
        dd->draw_display_background();
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
          dd->write_drive_direction(DRIVE_DIRECTION::FORWARD);
        } else if (input[2] == 'b') {
          carState.DriveDirection.set(DRIVE_DIRECTION::BACKWARD);
          dd->write_drive_direction(DRIVE_DIRECTION::BACKWARD);
        } else {
          carState.Speed.set(atoi(&input[1]));
          dd->write_speed();
        }
        break;
      case 'b':
        dd->write_bat(atof(&input[1]));
        break;
      case 'p':
        dd->write_pv(atof(&input[1]));
        break;
      case 'm':
        dd->write_motor(atof(&input[1]));
        break;
      case 'a':
        accValue = atoi(&input[1]);
        dd->write_acceleration(accValue);
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
        if (String("off") == String(&input[2])) {
          debug_printf("%s:%s-->off\n", input.c_str(), &input[2]);
          dd->arrow_increase(false);
        } else {
          debug_printf("%s:%s-->on\n", input.c_str(), &input[2]);
          dd->arrow_increase(true);
        }
        break;
      case 'd':
        if (String("off") == String(&input[2])) {
          debug_printf("%s:%s-->off\n", input.c_str(), &input[2]);
          dd->arrow_decrease(false);
        } else {
          debug_printf("%s:%s-->on\n", input.c_str(), &input[2]);
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
      case 'l':
        dd->light1OnOff();
        break;
      case 'L':
        dd->light2OnOff();
        break;
      case 'c':
        if (input[2] == 's') {
          dd->constant_drive_mode_set(CONSTANT_MODE::SPEED);
          dd->constant_drive_mode_show();
        } else if (input[2] == 'p') {
          dd->constant_drive_mode_set(CONSTANT_MODE::POWER);
          dd->constant_drive_mode_show();
        } else {
          dd->constant_drive_mode_set(CONSTANT_MODE::NONE);
          dd->constant_drive_mode_show();
        }
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
    for (int pin = 0; pin < PCF8574_NUM_PORTS; pin++) {
      int port = (devNr << 4) + pin;
      int idx = devNr * 8 + pin;
      if (ioExt.pins[idx].value == 0) {
        printf("%s: SET 0x%02x\n", ioExt.pins[idx].name.c_str(), port);
      }
    }
  }
  printf("POT0 (accel)= %4d, POT1 (recup)= %4d\n", dac.get_pot(DAC::pot_chan::POT_CHAN0), dac.get_pot(DAC::pot_chan::POT_CHAN1));
}
