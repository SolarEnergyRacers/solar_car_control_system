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
#include <ESP32Time.h>
#include <I2CBus.h>
#include <RtcDateTime.cpp>
#include <Wire.h> // I2C

#include <ADC.h>
#include <CANBus.h>
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
#include <IOExtHandler.h>
#include <Indicator.h>
#include <RTC.h>
#include <SDCard.h>
#include <system.h>

#if ADC_ON
extern ADC adc;
#endif
extern CANBus can;
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
#if RTC_ON
extern RTC rtc;
extern ESP32Time esp32time;
#endif

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

template <size_t N> int splitString(string (&arr)[N], string str) {
  int n = 0;
  istringstream iss(str);
  for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>() && n < N; ++it, ++n)
    arr[n] = *it;
  return n;
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
        } else if (Serial2.available()) {
          input = Serial2.readString();
          Serial2.flush();
        }
        if (input[0] < 0x20 || input[0] > 0x7F) // ignore
          break;

        if (input.endsWith("\n")) {
          input = input.substring(0, input.length() - 1);
        }
        if (input.endsWith("\r")) {
          input = input.substring(0, input.length() - 1);
        }

        if (input.length() > 0 && commands.find(input[0], 0) == -1) {
          input = "h"; // help
        }
        if (input.length() == 0)
          break;

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
        case 'M':
          sdCard.mount();
          break;
        case 'P':
          sdCard.directory();
          break;
        case 'U':
          sdCard.unmount();
          break;
        case 'H':
          memory_info();
          break;
        case 'b':
          can.verboseModeCan = !can.verboseModeCan;
          console << "set verboseMode for canbus: " << can.verboseModeCan << "\n";
          break;
        case 'i':
          console << "Received: '" << input << "' --> ";
          if (input[1] == 'i') {
            ioExt.verboseModeDigitalIn = !ioExt.verboseModeDigitalIn;
            console << "set verboseModeDigitalIn: " << ioExt.verboseModeDigitalIn << "\n";
          } else if (input[1] == 'o') {
            ioExt.verboseModeDigitalOut = !ioExt.verboseModeDigitalOut;
            console << "set verboseModeDigitalOut: " << ioExt.verboseModeDigitalOut << "\n";
          } else if (input[1] == 'a') {
            adc.verboseModeADC = !adc.verboseModeADC;
            console << "set verboseModeADC: " << adc.verboseModeADC << "\n";
          } else if (input[1] == 'd') {
            dac.verboseModeDAC = !dac.verboseModeDAC;
            console << "set verboseModeDAC: " << dac.verboseModeDAC << "\n";
          } else if (input[1] == 'c') {
            carControl.verboseMode = !carControl.verboseMode;
            console << "set verboseMode for acc-/dec-controls: " << carControl.verboseMode << "\n";
          } else if (input[1] == 'R') {
            console << ioExt.re_init() << "\n";
            msg = ioExt.re_init();
            console << msg << "\n";
          } else {
            ioExt.readAllPins();
            console << carState.printIOs("", true, false) << "\n";
          }
          break;
        case 'I':
          console << "Received: '" << input << "' -->  i2cBus.scan_i2c_devices()\n";
          i2cBus.scan_i2c_devices();
          break;
        // -------------- chase car commands
        case 'k':
#if CARSPEED_ON
          console << "Received: '" << input << "' --> ";
          if (input[1] == 'v') {
            carSpeed.verboseModePID = !carSpeed.verboseModePID;
          } else {
            string arr[4];
            int count = splitString(arr, &input[1]);
            if (count == 0) {
              console << "PID parameters: ";
            } else {
              float Kp = atof(arr[0].c_str());
              float Ki = atof(arr[1].c_str());
              float Kd = atof(arr[2].c_str());
              carSpeed.update_pid(Kp, Ki, Kd);
              console << "PID set parameters: ";
            }
            console << "Kp=" << carState.Kp << ", Ki=" << carState.Ki << ", Kd=" << carState.Kd << "\n";
          }
#else
          console << "Car speed control deactivated\n";
#endif
          break;
        case 'B':
          if (input[1] == '\0') {
            console << "Serial2 baudrate=" << carState.Serial2Baudrate << "\n";
          } else {
            carState.Serial2Baudrate = atof(&input[1]);
            Serial2.end();
            Serial2.begin(carState.Serial2Baudrate, SERIAL_8N1, SERIAL2_RX, SERIAL2_TX);
            console << "Restart Serial2 with baudrate=" << carState.Serial2Baudrate << "\n";
          }
          break;
        case 'T': {
#if RTC_ON
          console << "Received: '" << input.c_str();
          string arr[6];
          int count = splitString(arr, &input[1]);
          if (count == 0) {
            console << "' --> DateTime: ";
          } else {
            int yy = atoi(arr[0].c_str());
            int mm = atoi(arr[1].c_str());
            int dd = atoi(arr[2].c_str());
            int hh = atoi(arr[3].c_str());
            int MM = atoi(arr[4].c_str());
            int ss = atoi(arr[5].c_str());
            uint16_t days = DaysSinceFirstOfYear2000<uint16_t>(yy, mm, dd);
            uint64_t seconds = SecondsIn<uint64_t>(days, hh, MM, ss);
            RtcDateTime dateTime = RtcDateTime(seconds);
            rtc.write_rtc_datetime(dateTime);
            esp32time.setTime(ss, MM, hh, dd, mm, yy);
            console << "' --> Set dateTime to: ";
          }
          console << getDateTime() << ", uptime: " << getTimeStamp() << "\n";
#else
          console << "RTC deactivated\n";
#endif
        } break;
        case '-':
          console << "Received: '" << input << "' -->  carControl.adjust_paddles(" << carState.PaddleAdjustCounter << ")\n";
          carControl.adjust_paddles(carState.PaddleAdjustCounter); // manually adjust paddles (3s handling time)
          break;
        case 'F':
          if (input[1] == '-') {
            carState.Fan = false;
            carState.getPin(PinFanOut)->value = 0;
          } else {
            carState.Fan = true;
            carState.getPin(PinFanOut)->value = 1;
          }
          console << "Received: '" << input << "' -->  carState.Fan=" << carState.Fan << "\n";
          break;
        case 'G':
          console << "Received: '" << input << "' -->  carState.GreenLight=" << carState.GreenLight << "\n";
          break;
        case 'a':
          if (input[1] == 'u') {
            carState.SpeedArrow = SPEED_ARROW::INCREASE;
            console << "set arrow increase: " << ioExt.verboseModeDigitalIn << "\n";
          } else if (input[1] == 'd') {
            carState.SpeedArrow = SPEED_ARROW::DECREASE;
            console << "set verboseModeDigitalOut: " << ioExt.verboseModeDigitalOut << "\n";
          } else if (input[1] == 'o') {
            carState.SpeedArrow = SPEED_ARROW::OFF;
            console << "set verboseModeADC: " << adc.verboseModeADC << "\n";
          }
          console << "Received: '" << input << "' -->  carState.SpeedArrow=" << SPEED_ARROW_str[(int)(carState.SpeedArrow)] << "\n";
          break;
        case ':':
          carState.DriverInfoType = INFO_TYPE::INFO;
          carState.DriverInfo = &input[1];
          console << "Received: '" << input << "' -->  carState.DriverInfo " << INFO_TYPE_str[(int)carState.DriverInfoType] << ": "
                  << carState.DriverInfo << "\n";
          break;
        case '!':
          carState.DriverInfoType = INFO_TYPE::WARN;
          carState.DriverInfo = &input[1];
          console << "Received: '" << input << "' -->  carState.DriverInfo " << INFO_TYPE_str[(int)carState.DriverInfoType] << ": "
                  << carState.DriverInfo << "\n";
          break;
        // -------------- steering wheel input element emulators
        case '<':
          indicator.setIndicator(INDICATOR::LEFT);
          indicatorHandler();
          console << "Received: '" << input << "' -->  carState.Indicator=" << INDICATOR_str[(int)(carState.Indicator)] << "\n";
          break;
        case '>':
          indicator.setIndicator(INDICATOR::RIGHT);
          indicatorHandler();
          console << "Received: '" << input << "' -->  carState.Indicator=" << INDICATOR_str[(int)(carState.Indicator)] << "\n";
          break;
        case 'w':
          indicator.setIndicator(INDICATOR::WARN);
          indicatorHandler();
          console << "Received: '" << input << "' -->  carState.Indicator=" << INDICATOR_str[(int)(carState.Indicator)] << "\n";
          break;
        case 'l':
          if (input[1] == '-') {
            carState.Light = LIGHT::OFF;
          } else {
            carState.Light = LIGHT::L1;
          }
          light_switch();
          console << "Received: '" << input << "' -->  carState.Light=" << LIGHT_str[(int)(carState.Light)] << "\n";
          break;
        case 'L':
          if (input[1] == '-') {
            carState.Light = LIGHT::OFF;
          } else {
            carState.Light = LIGHT::L2;
          }
          light_switch();
          console << "Received: '" << input << "' -->  carState.Light=" << LIGHT_str[(int)(carState.Light)] << "\n";
          break;
        case 'c':
          if (input[2] == '-') {
            carState.ConstantModeOn = false; // #SAFETY#: deceleration unlock const mode
          } else if (input[2] == 's') {
            carState.ConstantMode = CONSTANT_MODE::SPEED;
            carState.ConstantModeOn = true; // #SAFETY#: deceleration unlock const mode
          } else if (input[2] == 'p') {
            carState.ConstantMode = CONSTANT_MODE::POWER;
            carState.ConstantModeOn = true; // #SAFETY#: deceleration unlock const mode
          } else {
            carState.ConstantModeOn = true; // #SAFETY#: deceleration unlock const mode
          }
          console << "Received: '" << input << "' -->  carState.ConstantMode - " << CONSTANT_MODE_str[(int)(carState.ConstantMode)]
                  << " On: " << carState.ConstantModeOn << "\n";
          break;
        default:
          console << "Received: '" << input << "'\n";
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
  int16_t valueDec = adc.STW_DEC;
  int16_t valueAcc = adc.STW_ACC;
  ss << fmt::format("ADC: dec={:5d}  acc={:5d}\n", valueDec, valueAcc);
#endif
#if DAC_ON
  ss << fmt::format("DAC: POT-0 (accel)= {:4d}, POT-1 (recup)= {:4d}\n", dac.get_pot(DAC::pot_chan::POT_CHAN0),
                    dac.get_pot(DAC::pot_chan::POT_CHAN1));
#endif
  // for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
  //   for (int pinNr = 0; pinNr < MCP23017_NUM_PORTS; pinNr++) {
  //     CarStatePin *pin = carState.getPin(devNr, pinNr);
  //     ss << fmt::format("{:20s} {:#04x}: {%10s} Pin ", pin->name, pin->port, pin->mode);
  //     if (pin->value == 0) {
  //       ss << "SET\n";
  //     } else {
  //       ss << "UNSET\n";
  //     }
  //   }
  // }
  return ss.str();
}
