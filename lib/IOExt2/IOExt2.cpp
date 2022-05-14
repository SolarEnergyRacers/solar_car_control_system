//
// MCP23017 I/O Extension over I2C
//
#include <definitions.h>
#if IOEXT2_ON

#include <stdio.h>

// standard libraries
#include <Console.h>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <inttypes.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <MCP23017.h> // MCP23017
#include <Wire.h>     // I2C

#include <CarControl.h>
#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
#include <I2CBus.h>
#include <IOExt2.h>

extern Console console;
extern I2CBus i2cBus;
extern Indicator indicator;
extern IOExt2 ioExt;
extern CarState carState;
extern CarControl carControl;
extern bool systemOk;

#if STEERINGWHEEL_BOARD_V1
CarStatePin CarState::pins[] = { // IOExtDev0-PortA
    {0x00, INPUT_PULLUP, 1, 1, false, 0l, PinBatOnOff, batteryOnOffHandler},
    {0x01, INPUT_PULLUP, 1, 1, false, 0l, PinPvOnOff, pvOnOffHandler},
    {0x02, INPUT_PULLUP, 1, 1, false, 0l, PinMcOnOff, mcOnOffHandler},
    {0x03, INPUT_PULLUP, 1, 1, false, 0l, PinEcoPower, ecoPowerHandler},
    {0x04, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY04, NULL},
    {0x05, INPUT_PULLUP, 1, 1, false, 0l, PinFwdBwd, fwdBwdHandler},
    {0x06, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY06, NULL},
    {0x07, OUTPUT, 0, 0, false, 0l, PinFanOut, NULL},
    // IOExtDev0-PortB
    {0x08, OUTPUT, 0, 0, false, 0l, PinIndicatorOutLeft, NULL},
    {0x09, OUTPUT, 0, 0, false, 0l, PinIndicatorOutRight, NULL},
    {0x0a, OUTPUT, 0, 0, false, 0l, PinHornOut, NULL},
    {0x0b, OUTPUT, 0, 0, false, 0l, PinHeadLightOut, NULL},
    {0x0c, OUTPUT, 0, 0, false, 0l, PinLightOut, NULL},
    {0x0d, INPUT_PULLUP, 1, 1, false, 0l, PinBreakPedal, breakPedalHandler},
    {0x0e, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY14, NULL},
    {0x0f, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY15, NULL},
    // IOExtDev1-PortA
    {0x10, INPUT_PULLUP, 1, 1, false, 0l, PinIndicatorBtnLeft, indicatorHandler},
    {0x11, INPUT_PULLUP, 1, 1, false, 0l, PinHeadLight, headLightHandler},
    {0x12, INPUT_PULLUP, 1, 1, false, 0l, PinLight, lightHandler},
    {0x13, INPUT_PULLUP, 1, 1, false, 0l, PinHorn, hornHandler},
    {0x14, INPUT_PULLUP, 1, 1, false, 0l, PinNextScreen, nextScreenHandler},
    {0x15, INPUT_PULLUP, 1, 1, false, 0l, PinConstantMode, constantModeHandler},
    {0x16, INPUT_PULLUP, 1, 1, false, 0l, PinIndicatorBtnRight, indicatorHandler},
    {0x17, INPUT_PULLUP, 1, 1, false, 0l, PinConstantModeOn, constantModeOnOffHandler}, // #SAFETY#: deceleration unlock const mode
    // IOExtDev1-PortB
    {0x18, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY31, NULL},
    {0x19, INPUT_PULLUP, 1, 1, false, 0l, PinReserve1, NULL},
    {0x1a, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY33, NULL},
    {0x1b, INPUT_PULLUP, 1, 1, false, 0l, PinPaddleAdjust, paddleAdjustHandler}, // #SAFETY#: lock acceleration
    {0x1c, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY35, NULL},
    {0x1d, INPUT_PULLUP, 1, 1, false, 0l, PinSdCardDetect, sdCardDetectHandler},
    {0x1e, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY37, NULL},
    {0x1f, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY38, NULL}};
#endif
#if STEERINGWHEEL_BOARD_V2
CarStatePin CarState::pins[] = { // IOExtDev0-PortA
    {0x00, INPUT_PULLUP, 1, 1, false, 0l, PinBatOnOff, batteryOnOffHandler},
    {0x01, INPUT_PULLUP, 1, 1, false, 0l, PinPvOnOff, pvOnOffHandler},
    {0x02, INPUT_PULLUP, 1, 1, false, 0l, PinMcOnOff, mcOnOffHandler},
    {0x03, INPUT_PULLUP, 1, 1, false, 0l, PinEcoPower, ecoPowerHandler},
    {0x04, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY04, NULL},
    {0x05, INPUT_PULLUP, 1, 1, false, 0l, PinFwdBwd, fwdBwdHandler},
    {0x06, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY06, NULL},
    {0x07, OUTPUT, 0, 0, false, 0l, PinFanOut, NULL},
    // IOExtDev0-PortB
    {0x08, OUTPUT, 0, 0, false, 0l, PinIndicatorOutLeft, NULL},
    {0x09, OUTPUT, 0, 0, false, 0l, PinIndicatorOutRight, NULL},
    {0x0a, OUTPUT, 0, 0, false, 0l, PinHornOut, NULL},
    {0x0b, OUTPUT, 0, 0, false, 0l, PinHeadLightOut, NULL},
    {0x0c, OUTPUT, 0, 0, false, 0l, PinLightOut, NULL},
    {0x0d, INPUT_PULLUP, 1, 1, false, 0l, PinBreakPedal, breakPedalHandler},
    {0x0e, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY14, NULL},
    {0x0f, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY15, NULL},
    // IOExtDev1-PortA
    {0x10, INPUT_PULLUP, 1, 1, false, 0l, PinIndicatorBtnLeft, indicatorHandler},
    {0x11, INPUT_PULLUP, 1, 1, false, 0l, PinHeadLight, headLightHandler},
    {0x12, INPUT_PULLUP, 1, 1, false, 0l, PinConstantModeOff, constantModeOffHandler},
    {0x13, INPUT_PULLUP, 1, 1, false, 0l, PinHorn, hornHandler},
    {0x14, INPUT_PULLUP, 1, 1, false, 0l, PinNextScreen, nextScreenHandler},
    {0x15, INPUT_PULLUP, 1, 1, false, 0l, PinConstantMode, constantModeHandler},
    {0x16, INPUT_PULLUP, 1, 1, false, 0l, PinIndicatorBtnRight, indicatorHandler},
    {0x17, INPUT_PULLUP, 1, 1, false, 0l, PinConstantModeOn, constantModeOnHandler}, // #SAFETY#: deceleration unlock const mode
    // IOExtDev1-PortB
    {0x18, INPUT_PULLUP, 1, 1, false, 0l, PinPaddleAdjust, paddleAdjustHandler}, // #SAFETY#: lock acceleration
    {0x19, INPUT_PULLUP, 1, 1, false, 0l, PinLight, lightHandler},
    {0x1a, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY33, NULL},
    {0x1b, INPUT_PULLUP, 1, 1, false, 0l, PinIncrease, increaseHandler},
    {0x1c, INPUT_PULLUP, 1, 1, false, 0l, PinDecrease, decreaseHandler},
    {0x1d, INPUT_PULLUP, 1, 1, false, 0l, PinSdCardDetect, sdCardDetectHandler},
    {0x1e, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY37, NULL},
    {0x1f, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY38, NULL}};
#endif
// ------------------
// FreeRTOS functions

volatile bool ioInterruptRequest;
void IRAM_ATTR ioExt_interrupt_handler() { ioInterruptRequest = true; };

void IOExt2::re_init() { init(); }

void IOExt2::init() {
  string s;
  for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
    console << "[?] Init IOExt2 " << devNr << "\n";
    xSemaphoreTakeT(i2cBus.mutex);
    ioExt.IOExtDevs[devNr].init();
    ioExt.IOExtDevs[devNr].interruptMode(MCP23017InterruptMode::Or);
    ioExt.IOExtDevs[devNr].interrupt(MCP23017Port::A, CHANGE); // FALLING);
    ioExt.IOExtDevs[devNr].interrupt(MCP23017Port::B, CHANGE); // FALLING);
    xSemaphoreGive(i2cBus.mutex);
    for (int pinNr = 0; pinNr < MCP23017_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      carState.idxOfPin.insert(pair<string, int>{pin->name, getIdx(devNr, pinNr)});
      xSemaphoreTakeT(i2cBus.mutex);
      ioExt.IOExtDevs[devNr].pinMode(pinNr, pin->mode);
      // if (pin->mode != OUTPUT) {
      //   pin->value = pin->oldValue = ioExt.IOExtDevs[devNr].digitalRead(pinNr);
      // }
      xSemaphoreGive(i2cBus.mutex);
      int dev = pin->port >> 4;
      int devPin = pin->port & 0xf;
      s = fmt::format("  {:#04x} [{:02d}] dev:{:02d}, devpin:{:02d}, mode:{}, value={} ({})\n", pin->port, carState.getIdx(pin->name), dev,
                      devPin, pin->mode != OUTPUT ? "INPUT " : "OUTPUT", pin->value, pin->name.c_str());
      console << s;
    }
    ioExt.IOExtDevs[devNr].clearInterrupts();
    console << "[v] " << getName() << "[" << devNr << "] initialized.\n";
  }
  ioInterruptRequest = false;
  pinMode(I2C_INTERRUPT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(I2C_INTERRUPT), ioExt_interrupt_handler, CHANGE);
  set_SleepTime(200);
}

void IOExt2::exit(void) {
  // TODO
}
// ------------------

void IOExt2::handleIoInterrupt() {
  if (isInInterruptHandler) {
    console << "Jump out of interrupt handler at " << millis() << "\n";
    return;
  }
  isInInterruptHandler = true;
  // debug_printf("Do interrupt handler at %ld\n", millis());
  readAll(true);
  // debug_printf("clear interrupts   %ld\n", millis());
  xSemaphoreTakeT(i2cBus.mutex);
  for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
    // uint8_t portA, portB;
    // //uint8_t allPins = ioExt.IOExtDevs[devNr].read();
    // ioExt.IOExtDevs[devNr].clearInterrupts(portA, portB);
    // debug_printf("Jump in interrupt handler at %ld::\tdevNr: %d -- pA %02x, pB %02x\n", millis(), devNr, portA, portB);
    ioExt.IOExtDevs[devNr].clearInterrupts();
  }
  xSemaphoreGive(i2cBus.mutex);
  isInInterruptHandler = false;
}

void IOExt2::readAll(bool deltaOnly, bool forced) {
  // string normalColor = "\033[0;39m";
  // string highLightColorChg = "\033[1;36m"; // blue
  list<void (*)()> pinHandlerList;
  for (int devNr = 0; devNr < MCP23017_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < MCP23017_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->mode != OUTPUT) {

        xSemaphoreTakeT(i2cBus.mutex);
        pin->value = ioExt.IOExtDevs[devNr].digitalRead(pinNr);
        xSemaphoreGive(i2cBus.mutex);
        // if (pin->value == 0)
        //   printf("%s %02d: dev: %d, pin:%x -- value:%d%s\n", highLightColorChg.c_str(), pin->port, devNr, pinNr, pin->value,
        //          normalColor.c_str());
        // else
        //   printf(" %02d: dev: %d, pin:%x -- value:%d\n", pin->port, devNr, pinNr, pin->value);
        if (pin->handlerFunction != NULL && (pin->value != pin->oldValue || !pin->inited || forced)) {
          pin->inited = true;
          pinHandlerList.push_back(pin->handlerFunction);
          pin->oldValue = pin->value;
        }
      }
    }
  }
  string outString = carState.printIOs("", true, deltaOnly);
  if (outString.length() > 0)
    console << fmt::format("{}\n", outString.c_str());
  // avoid multi registration:
  pinHandlerList.unique();
  // call all handlers for changed pins
  for (void (*pinHandler)() : pinHandlerList) {
    pinHandler();
  }
  pinHandlerList.clear();
}

void IOExt2::setPortMode(int port, uint8_t mode) {
  // get device & port
  int devNr = port >> 4;
  int pin = port & 0xf;
  xSemaphoreTakeT(i2cBus.mutex);
  ioExt.IOExtDevs[devNr].pinMode(pin, mode);
  xSemaphoreGive(i2cBus.mutex);
}

void IOExt2::setPort(int port, bool value) {
  // get device & port
  int devNr = port >> 4;
  int pin = port & 0xf;
  // debug_printf("Set BOOL--port:0x%02x--devNr:%d--pin:%d--value:%d---%ld-\n", port, devNr, pin, value, millis());
  xSemaphoreTakeT(i2cBus.mutex);
  ioExt.IOExtDevs[devNr].digitalWrite(pin, value ? 1 : 0);
  xSemaphoreGive(i2cBus.mutex);
}

int IOExt2::getPort(int port) {
  // get device & port
  int devNr = port >> 4;
  int pin = port & 0xf;
  xSemaphoreTakeT(i2cBus.mutex);
  int value = ioExt.IOExtDevs[devNr].digitalRead(pin);
  xSemaphoreGive(i2cBus.mutex);
  console << fmt::sprintf("Get BOOL--port:0x%02x--devNr:%d--pin:%d--value:%d---%ld-\n", port, devNr, pin, value, millis());
  console << fmt::sprintf("0x%02x [%d|%d]: %d\n", port, devNr, pin, value);
  return value;
}

void IOExt2::task() {

  // polling loop
  while (1) {
    if (systemOk) {
      // handle INPUT pin interrupts
      if (ioInterruptRequest) {
        // console << "ioInterruptRequest " << millis() << "\n";
        handleIoInterrupt();
        ioInterruptRequest = false;
      }
      // update OUTPUT pins
      for (auto &pin : carState.pins) {
        if (pin.mode == OUTPUT && (pin.oldValue != pin.value || !pin.inited)) {
          pin.oldValue = pin.value;
          pin.inited = true;
          // debug_printf("Set %02x to %d\n", pin.port, pin.value);
          setPort(pin.port, pin.value);
        }
      }
    }
    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}

// IO pin handler -----------------------------------------

void batteryOnOffHandler() {
  carState.BatteryOn = carState.getPin(PinBatOnOff)->value == 1;
  console << "Battery " << (carState.BatteryOn ? "On" : "Off") << "\n";
}

void pvOnOffHandler() {
  carState.PhotoVoltaicOn = carState.getPin(PinPvOnOff)->value == 1;
  console << "PV " << (carState.PhotoVoltaicOn ? "On" : "Off") << "\n";
}

void mcOnOffHandler() {
  carState.MotorOn = carState.getPin(PinMcOnOff)->value == 1;
  console << "MC " << (carState.MotorOn ? "On" : "Off") << "\n";
}

void ecoPowerHandler() {
  carState.EcoOn = carState.getPin(PinEcoPower)->value == 1;
  console << "EcoMowerMode " << (carState.EcoOn ? "Eco" : "Power") << "\n";
}

void fwdBwdHandler() {
  carState.DriveDirection = carState.getPin(PinFwdBwd)->value == 1 ? DRIVE_DIRECTION::FORWARD : DRIVE_DIRECTION::BACKWARD;
  console << "Direction " << (carState.DriveDirection == DRIVE_DIRECTION::FORWARD ? "Forward" : "Backward") << "\n";
}

void breakPedalHandler() {
  if (VCC_12V) { // break pedal relais works only with 12V
    carState.BreakPedal = carState.getPin(PinBreakPedal)->value == 0;
  } else { // UCC_5V
    carState.BreakPedal = carState.getPin(PinBreakPedal)->value == 1;
  }
  console << "Break pedal pressed " << (carState.BreakPedal ? "pressed" : "released") << "\n";
}

void indicatorHandler() {
  int indiLeft = carState.getPin(PinIndicatorBtnLeft)->value;
  int indiRight = carState.getPin(PinIndicatorBtnRight)->value;
  if (indiLeft == 0 || indiRight == 0) {
    indicator.setIndicator(indiLeft, indiRight);
  }
  carState.getPin(PinIndicatorBtnLeft)->oldValue = carState.getPin(PinIndicatorBtnLeft)->value;
  carState.getPin(PinIndicatorBtnRight)->oldValue = carState.getPin(PinIndicatorBtnRight)->value;
}

void hornHandler() {
  int value = carState.getPin(PinHorn)->value;
  carState.getPin(PinHornOut)->value = (value == 0);
  console << "Horn " << (value == 0 ? "On" : "Off") << "\n";
}

void lightHandler() {
  int value = carState.getPin(PinLight)->value;
  if (value == 0) {
    console << "Light toggle\n";
    ;
    if (carState.Light == LIGHT::L1) {
      carState.Light = LIGHT::OFF;
      carState.getPin(PinLightOut)->value = 0;
      carState.getPin(PinHeadLightOut)->value = 0;
    } else {
      carState.Light = LIGHT::L1;
      carState.getPin(PinLightOut)->value = 1;
    }
  }
}

void headLightHandler() {
  int value = carState.getPin(PinHeadLight)->value;
  if (value == 0) {
    console << "Drive Light toggle\n";
    if (carState.Light == LIGHT::L2) {
      carState.Light = LIGHT::L1;
      carState.getPin(PinLightOut)->value = 1;
      carState.getPin(PinHeadLightOut)->value = 0;
    } else {
      carState.Light = LIGHT::L2;
      carState.getPin(PinLightOut)->value = 1;
      carState.getPin(PinHeadLightOut)->value = 1;
    }
  }
}

void nextScreenHandler() {
    if( !systemOk) return;

  if (carState.getPin(PinNextScreen)->value == 0) {
    switch (carState.displayStatus) {
    case DISPLAY_STATUS::ENGINEER_RUNNING:
      carState.displayStatus = DISPLAY_STATUS::DRIVER_SETUP;
      console << "Switch Next Screen toggle: switch from eng --> driver\n";
      break;
    case DISPLAY_STATUS::DRIVER_RUNNING:
      carState.displayStatus = DISPLAY_STATUS::ENGINEER_SETUP;
      console << "Switch Next Screen toggle: switch from driver --> engineer\n";
      break;
    default:
      break;
    }
  }
}

void constantModeOnHandler() {
  if (!systemOk)
    return;
  if (carState.getPin(PinConstantModeOn)->value == 0) {
    console << "ConstantMode ON\n";
    carState.TargetSpeed = carState.Speed;                                       // unit: km/h
    carState.TargetPower = carState.MotorCurrent * carState.MotorVoltage / 1000; // unit: kW
    carState.ConstantModeOn = true;                                              // #SAFETY#: deceleration unlock const mode
  }
}

void constantModeOffHandler() {
  if (!systemOk)
    return;
  if (carState.getPin(PinConstantModeOff)->value == 0) {
    if (carState.ConstantModeOn) {
      console << "ConstantMode OFF\n";
      carState.ConstantModeOn = false; // #SAFETY#: deceleration unlock const mode
    }
  }
}
void constantModeHandler() {
  if (!systemOk)
    return;
  if (carState.getPin(PinConstantMode)->value == 0) {
    console << "Constant mode toggle\n";
    if (carState.ConstantMode == CONSTANT_MODE::POWER) {
      carState.TargetPower = 0;
      carState.ConstantMode = CONSTANT_MODE::SPEED;
    } else {
      carState.TargetSpeed = 0;
      carState.ConstantMode = CONSTANT_MODE::POWER;
    }
    carState.TargetSpeed = carState.Speed;                                       // unit: km/h
    carState.TargetPower = carState.MotorCurrent * carState.MotorVoltage / 1000; // unit: kW
  }
}

void paddleAdjustHandler() {
  if (!systemOk)
    return;
  if (carState.getPin(PinPaddleAdjust)->value == 0) {
    console << "Request Paddle Adjust\n";
    carControl.adjust_paddles(carState.PaddleAdjustCounter); // manually adjust paddles (3s handling time)
  }
}

void sdCardDetectHandler() {
  carState.SdCardDetect = carState.getPin(PinSdCardDetect)->value == 1;
  if (carState.SdCardDetect) {
    console << "SD card detected.\n";
  } else {
    console << "SD card removed.\n";
  }
}

void decreaseHandler() {
  if (!systemOk)
    return;
  console << "Decrease constant mode target.\n";
  carState.TargetSpeed -= carState.ConstSpeedIncrease;
  if (carState.TargetSpeed < 0)
    carState.TargetSpeed = 0;

  carState.TargetPower -= carState.ConstPowerIncrease;
  if (carState.TargetPower < 0)
    carState.TargetPower = 0;
}

void increaseHandler() {
  if (!systemOk)
    return;
  console << "Increase constant mode target.\n";
  carState.TargetSpeed += carState.ConstSpeedIncrease;
  if (carState.TargetSpeed > 111) // only until 111km/h
    carState.TargetSpeed = 111;

  carState.TargetPower += carState.ConstPowerIncrease;
  if (carState.TargetPower > 4500) // only until 5kW
    carState.TargetPower = 4500;
}
// end IO pin handler -----------------------------------------
#endif
