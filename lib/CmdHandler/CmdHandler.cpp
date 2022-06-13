//
// Command Receiver and Handler
//
// reads commands from serial console and deploy it
#include <definitions.h>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <iostream>
#include <iterator>
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
#include <CarSpeed.h>
#include <CarState.h>
#include <CarStatePin.h>
#include <CmdHandler.h>
#include <Console.h>
#include <DAC.h>
#include <Display.h>
#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
#include <IOExt.h>
#include <Indicator.h>
#include <SDCard.h>
#include <system.h>

#if ADC_ON
extern ADC adc;
#endif
extern I2CBus i2cBus;
#if DAC_ON
extern DAC dac;
#endif
extern IOExt ioExt;
extern I2CBus i2cBus;
extern Indicator indicator;
extern CarState carState;
extern CarSpeed carSpeed;
extern CarControl carControl;
extern DriverDisplay driverDisplay;
extern EngineerDisplay engineerDisplay;
extern SDCard sdCard;
extern Console console;

using namespace std;

// ------------------
// FreeRTOS functions

string CmdHandler::re_init() { return init(); }

string CmdHandler::init() {
  bool hasError = false;
  console << "[  ] Init 'CmdHandler'...\n";
  // nothing to do, i2c bus is getting initialized externally
  return fmt::format("[{}] CmdHandler initialized.", hasError ? "--" : "ok");
}

void CmdHandler::exit() {
  // TODO
}
// ------------------

template <size_t N> void splitString(string (&arr)[N], string str) {
  int n = 0;
  istringstream iss(str);
  for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>() && n < N; ++it, ++n)
    arr[n] = *it;
}

void CmdHandler::task() {
  string state, msg;
  while (1) {
    try {
      while (Serial.available() || Serial2.available()) {
        // read the incoming chars:
        String input = "";
        if (Serial.available()) {
          input = Serial.readString();
          Serial.flush();
        }
        if (Serial2.available()) {
          input = Serial2.readString();
          Serial2.flush();
        }

        if (input.endsWith("\n")) {
          input = input.substring(0, input.length() - 1);
        }
        if (input.endsWith("\r")) {
          input = input.substring(0, input.length() - 1);
        }

        if (input.length() < 1 || commands.find(input[0], 0) == -1) {
          input = "h"; // help
        }

        switch (input[0]) {
        // ---------------- controller commands
        case '\n':
          console << "\n";
          break;
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
          console << carState.print("Recent State") << "\n";
          break;
        case 'S':
          console << carState.print("Recent State:") << "\n";
          console << printSystemValues();
          break;
        case 'J':
          state = carState.serialize("Recent State");
          sdCard.write(state + "\n");
          console << state << "\n";
          break;
        case 'v':
          state = carState.csv("Recent State");
          sdCard.write(state);
          console << state;
          break;
        case 'V':
          state = carState.csv("Recent State", true); // with header
          sdCard.write(state);
          console << state;
          break;
        case 'P':
          sdCard.directory();
          break;
        case 'U':
          sdCard.unmount();
          break;
        case 'M':
          sdCard.mount();
          break;
        case 'H':
          memory_info();
          break;
        case 'i':
          if (input[1] == 'i') {
            ioExt.verboseModeIn = !ioExt.verboseModeIn;
            console << "Received: " << input << " -->  set ioExt.verboseModeIn: " << ioExt.verboseModeIn << "\n";
          } else if (input[1] == 'o') {
            ioExt.verboseModeOut = !ioExt.verboseModeOut;
            console << "Received: " << input << " -->  set ioExt.verboseModeOut: " << ioExt.verboseModeOut << "\n";
          } else if (input[1] == 'R') {
            console << "Received: " << input << " -->  ioExt.re_init()\n";
            msg = ioExt.re_init();
            console << msg << "\n";
          } else {
            console << "Received: " << input
                    << ": ioExt.readAll(false, false, "
                       ", true)\n";
            // ioExt.readAll(false, false, "", true);
            ioExt.readPins(PinReadMode::ALL);
          }
          break;
        case 'I':
          console << "Received: " << input << " -->  i2cBus.scan_i2c_devices()\n";
          i2cBus.scan_i2c_devices();
          break;
        // -------------- chase car commands
        case 'k': {
          string arr[4];
          splitString(arr, &input[1]);
          float Kp = atof(arr[0].c_str());
          float Ki = atof(arr[1].c_str());
          float Kd = atof(arr[2].c_str());
#if CARSPEED_ON
          carSpeed.update_pid(Kp, Ki, Kd);
#endif
          console << "PID set to Kp=" << carState.Kp << ", Ki=" << carState.Ki << ", Kd=" << carState.Kd << "\n";
        } break;
        case '-':
          console << "Received: " << input << " -->  carControl.adjust_paddles(" << carState.PaddleAdjustCounter << ")\n";
          carControl.adjust_paddles(carState.PaddleAdjustCounter); // manually adjust paddles (3s handling time)
          break;
        case 'u':
          if (input[1] == '-' || carState.SpeedArrow == SPEED_ARROW::INCREASE) {
            carState.SpeedArrow = SPEED_ARROW::OFF;
          } else {
            carState.SpeedArrow = SPEED_ARROW::INCREASE;
          }
          console << "Received: " << input << " -->  carState.SpeedArrow=" << SPEED_ARROW_str[(int)(carState.SpeedArrow)] << "\n";
          break;
        case 'd':
          if (input[1] == '-' || carState.SpeedArrow == SPEED_ARROW::DECREASE) {
            carState.SpeedArrow = SPEED_ARROW::OFF;
          } else {
            carState.SpeedArrow = SPEED_ARROW::DECREASE;
          }
          console << "Received: " << input << " -->  carState.SpeedArrow=" << SPEED_ARROW_str[(int)(carState.SpeedArrow)] << "\n";
          break;
        case ':':
          carState.DriverInfoType = INFO_TYPE::INFO;
          carState.DriverInfo = &input[1];
          console << "Received: " << input << " -->  carState.DriverInfo [" << INFO_TYPE_str[(int)carState.DriverInfoType]
                  << "]=" << carState.DriverInfo << "\n";
          break;
        case '!':
          carState.DriverInfoType = INFO_TYPE::WARN;
          carState.DriverInfo = &input[1];
          console << "Received: " << input << " -->  carState.DriverInfo [" << INFO_TYPE_str[(int)carState.DriverInfoType]
                  << "]=" << carState.DriverInfo << "\n";
          break;
        // -------------- steering wheel input element emulators
        case '<':
          indicator.setIndicator(INDICATOR::LEFT);
          console << "Received: " << input << " -->  carState.Light=" << INDICATOR_str[(int)(carState.Indicator)] << "\n";
          break;
        case '>':
          indicator.setIndicator(INDICATOR::RIGHT);
          console << "Received: " << input << " -->  carState.Light=" << INDICATOR_str[(int)(carState.Indicator)] << "\n";
          break;
        case 'w':
          indicator.setIndicator(INDICATOR::WARN);
          console << "Received: " << input << " -->  carState.Light=" << INDICATOR_str[(int)(carState.Indicator)] << "\n";
          break;
        case 'l':
          if (input[1] == '-') {
            carState.Light = LIGHT::OFF;
          } else {
            carState.Light = LIGHT::L1;
          }
          console << "Received: " << input << " -->  carState.Light=" << LIGHT_str[(int)(carState.Light)] << "\n";
          break;
        case 'L':
          if (input[1] == '-') {
            carState.Light = LIGHT::OFF;
          } else {
            carState.Light = LIGHT::L2;
          }
          console << "Received: " << input << " -->  carState.Light=" << LIGHT_str[(int)(carState.Light)] << "\n";
          break;
        case 'c':
          if (input[2] == 's') {
            carState.ConstantMode = CONSTANT_MODE::SPEED;
            carState.ConstantModeOn = true; // #SAFETY#: deceleration unlock const mode
          } else if (input[2] == 'p') {
            carState.ConstantMode = CONSTANT_MODE::POWER;
            carState.ConstantModeOn = true; // #SAFETY#: deceleration unlock const mode
          } else {
            carState.ConstantModeOn = !carState.ConstantModeOn; // #SAFETY#: deceleration unlock const mode
          }
          console << "Received: " << input << " -->  carState.ConstantMode - " << CONSTANT_MODE_str[(int)(carState.ConstantMode)]
                  << " On: " << carState.ConstantModeOn << "\n";
          break;
        default:
          console << "Received: " << input << "\n";
          console << "ERROR:: Unknown command '" << input << "' \n" << helpText << "\n";
          break;
        case 'h':
          console << helpText << "\n";
          break;
        }
      }
    } catch (exception &ex) {
      console << "Exception:" << ex.what() << "\n";
    }
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}

string CmdHandler::printSystemValues() {
  stringstream ss("");
#if ADC_ON
  int16_t valueDec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);
  ss << fmt::format("dec={:5d}  acc={:5d}\n", valueDec, valueAcc);
#endif

  for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < MCP23017_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->value == 0) {
        ss << fmt::format("{}: SET {:#04x}\n", pin->name, pin->port);
      }
    }
  }
#if DAC_ON
  ss << fmt::format("POT-0 (accel)= {:4d}, POT-1 (recup)= {:4d}\n", dac.get_pot(DAC::pot_chan::POT_CHAN0),
                    dac.get_pot(DAC::pot_chan::POT_CHAN1));
#endif
  return ss.str();
}
