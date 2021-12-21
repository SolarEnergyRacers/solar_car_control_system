//
// PCF8574 I/O Extension over I2C
//
#include <definitions.h>
#if IOEXT_ON

// standard libraries
#include <iostream>
#include <stdio.h>
#include <string>

#include <PCF8574.h> // PCF8574
#include <Wire.h>    // I2C

#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
#include <I2CBus.h>
#include <IOExt.h>

extern I2CBus i2cBus;
extern Indicator indicator;
extern IOExt ioExt;
extern CarState carState;
extern bool systemOk;

CarStatePin CarState::pins[] = { // IOExtDev0
    {0x00, INPUT_PULLUP, 1, 1, false, 0l, PinBatOnOff, batteryOnOffHandler},
    {0x01, INPUT_PULLUP, 1, 1, false, 0l, PinPvOnOff, pvOnOffHandler},
    {0x02, INPUT_PULLUP, 1, 1, false, 0l, PinMcOnOff, mcOnOffHandler},
    {0x03, INPUT_PULLUP, 1, 1, false, 0l, PinEcoPower, ecoPowerHandler},
    {0x04, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY06, NULL},
    {0x05, INPUT_PULLUP, 1, 1, false, 0l, PinFwdBwd, fwdBwdHandler},
    {0x06, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY07, NULL},
    {0x07, OUTPUT, 1, 1, false, 0l, PinRelais11, NULL},
    // IOExtDev1
    {0x10, OUTPUT, 1, 1, false, 0l, PinIndicatorOutLeft, NULL},
    {0x11, OUTPUT, 1, 1, false, 0l, PinIndicatorOutRight, NULL},
    {0x12, OUTPUT, 1, 1, false, 0l, PinRalais12, NULL},
    {0x13, OUTPUT, 1, 1, false, 0l, PinRelais31, NULL},
    {0x14, OUTPUT, 1, 1, false, 0l, PinRelais32, NULL},
    {0x15, INPUT_PULLUP, 1, 1, false, 0l, PinBreakPedal, breakPedalHandler},
    {0x16, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY19, NULL},
    {0x17, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY17, NULL},
    // IOExtDev2
    {0x20, INPUT_PULLUP, 1, 1, false, 0l, PinIndicatorBtnLeft, indicatorHandler},
    {0x21, INPUT_PULLUP, 1, 1, false, 0l, PinIndicatorBtnRight, indicatorHandler},
    {0x22, INPUT_PULLUP, 1, 1, false, 0l, PinHeadLight, headLightHandler},
    {0x23, INPUT_PULLUP, 1, 1, false, 0l, PinLight, lightHandler},
    {0x24, INPUT_PULLUP, 1, 1, false, 0l, PinHorn, hornHandler},
    {0x25, INPUT_PULLUP, 1, 1, false, 0l, PinNextScreen, nextScreenHandler},
    {0x26, INPUT_PULLUP, 1, 1, false, 0l, PinConstantMode, constantModeHandler},
    {0x27, INPUT_PULLUP, 1, 1, false, 0l, PinConstantModeOn, constantModeOnOffHandler},
    // IOExtDev3
    {0x30, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY31, NULL},
    {0x31, INPUT_PULLUP, 1, 1, false, 0l, PinReserve1, NULL},
    {0x32, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY33, NULL},
    {0x33, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY34, NULL},
    {0x34, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY35, NULL},
    {0x35, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY36, NULL},
    {0x36, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY37, NULL},
    {0x37, INPUT_PULLUP, 1, 1, false, 0l, PinDUMMY38, NULL}};

// ------------------
// FreeRTOS functions

volatile bool IOExt::ioInterruptRequest = false;

void IOExt::re_init() { init(); }

void IOExt::init() {
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    printf("[?] Init IOExt %u...\n", devNr);
    xSemaphoreTakeT(i2cBus.mutex);
    bool isError = ioExt.IOExtDevs[devNr].begin();
    xSemaphoreGive(i2cBus.mutex);
    if (isError) {
      printf("[x] Initialization of IOExt %u failed.\n", devNr);
    } else {
      char msg[100];
      for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
        CarStatePin *pin = carState.getPin(devNr, pinNr);
        carState.idxOfPin.insert(pair<string, int>{pin->name, getIdx(devNr, pinNr)});
        xSemaphoreTakeT(i2cBus.mutex);
        ioExt.IOExtDevs[devNr].pinMode(pinNr, pin->mode);
        // if (pin->mode != OUTPUT) {
        //   pin->value = pin->oldValue = ioExt.IOExtDevs[devNr].digitalRead(pinNr);
        // }
        xSemaphoreGive(i2cBus.mutex);
        snprintf(msg, 100, "  0x%02x [%02d], mode:%s, value=%d (%s)\n", pin->port, carState.getIdx(pin->name),
                 pin->mode != OUTPUT ? "INPUT " : "OUTPUT", pin->value, pin->name.c_str());
        printf(msg);
      }
      printf("[v] %s[%d] initialized.\n", getName().c_str(), devNr);
    }
  }
  set_SleepTime(400);
}

void IOExt::exit(void) {
  // TODO
}
// ------------------

void IOExt::handleIoInterrupt() {
  if (isInInterruptHandler) {
    debug_printf("Jump out of interrupt handler at %ld\n", millis());
    return;
  }
  isInInterruptHandler = true;
  debug_printf("Do interrupt handler at %ld\n", millis());
  readAll();
  isInInterruptHandler = false;
}

void IOExt::readAll() {
  debug_printf("Read all INPUT* IOs at %ld\n", millis());
  list<void (*)()> pinHandlerList;
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->mode != OUTPUT) {

        xSemaphoreTakeT(i2cBus.mutex);
        pin->value = ioExt.IOExtDevs[devNr].digitalRead(pinNr);
        xSemaphoreGive(i2cBus.mutex);

        // if (pin->handlerFunction != NULL) {
        if (pin->handlerFunction != NULL && (pin->value != pin->oldValue || !pin->inited)) {
          pin->inited = true;
          pinHandlerList.push_back(pin->handlerFunction);
          pin->oldValue = pin->value;
        }
      }
    }
  }
  debug_printf("%s", carState.printIOs("").c_str());
  // avoid multi registration:
  pinHandlerList.unique();
  // call all handlers for changed pins
  for (void (*pinHandler)() : pinHandlerList) {
    pinHandler();
  }
  pinHandlerList.clear();
}

void IOExt::setPortMode(int port, uint8_t mode) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;
  xSemaphoreTakeT(i2cBus.mutex);
  ioExt.IOExtDevs[idx].pinMode(pin, mode);
  xSemaphoreGive(i2cBus.mutex);
}

void IOExt::setPort(int port, bool value) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;
  // debug_printf("BOOL----------%ld-\n", millis());
  xSemaphoreTakeT(i2cBus.mutex);
  ioExt.IOExtDevs[idx].digitalWrite(pin, value ? 1 : 0);
  xSemaphoreGive(i2cBus.mutex);
}

int IOExt::getPort(int port) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;
  xSemaphoreTakeT(i2cBus.mutex);
  int value = ioExt.IOExtDevs[idx].digitalRead(pin);
  xSemaphoreGive(i2cBus.mutex);

  debug_printf("0x%02x [%d|%d]: %d\n", port, idx, pin, value);
  return value;
}

void IOExt::task() {

  // polling loop
  while (1) {
    if (systemOk) {
      // handle INPUT pin interrupts
      if (ioInterruptRequest) {
        debug_printf("ioInterruptRequest %ld\n", millis());
        handleIoInterrupt();
        ioInterruptRequest = false;
      }
      // update OUTPUT pins
      for (auto &pin : carState.pins) {
        if (pin.mode == OUTPUT && (pin.oldValue != pin.value || !pin.inited)) {
          pin.oldValue = pin.value;
          pin.inited = true;
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
  printf("Battery %s\n", (carState.BatteryOn ? "On" : "Off"));
}

void pvOnOffHandler() {
  carState.PhotoVoltaicOn = carState.getPin(PinPvOnOff)->value == 1;
  printf("PV %s\n", (carState.PhotoVoltaicOn ? "On" : "Off"));
}

void mcOnOffHandler() {
  carState.MotorOn = carState.getPin(PinMcOnOff)->value == 1;
  printf("MC %s\n", (carState.MotorOn ? "On" : "Off"));
}

void ecoPowerHandler() {
  carState.EcoOn = carState.getPin(PinEcoPower)->value == 1;
  printf("EcoMowerMode %s\n", (carState.EcoOn ? "Eco" : "Power"));
}

void fwdBwdHandler() {
  carState.DriveDirection = carState.getPin(PinFwdBwd)->value == 1 ? DRIVE_DIRECTION::FORWARD : DRIVE_DIRECTION::BACKWARD;
  printf("Direction %s\n", (carState.DriveDirection == DRIVE_DIRECTION::FORWARD ? "Forward" : "Backward"));
}

void breakPedalHandler() {
  carState.BreakPedal = carState.getPin(PinBreakPedal)->value == 1;
  printf("Break pedal pressed %s\n", (carState.BreakPedal ? "pressed" : "released"));
}

void indicatorHandler() {
  int indiLeft = carState.getPin(PinIndicatorBtnLeft)->value;
  int indiRight = carState.getPin(PinIndicatorBtnRight)->value;
  if (indiLeft == 0 || indiRight == 0) {
    indicator.setIndicator(indiLeft, indiRight);
  }
}

void hornHandler() {
  int value = carState.getPin(PinHorn)->value;
  printf("Horn %s\n", (value == 0 ? "On" : "Off"));
}

void lightHandler() {
  int value = carState.getPin(PinLight)->value;
  if (value == 0) {
    printf("Light toggle\n");
    if (carState.Light == LIGHT::L1) {
      carState.Light = LIGHT::OFF;
    } else {
      carState.Light = LIGHT::L1;
    }
  }
}

void headLightHandler() {
  int value = carState.getPin(PinHeadLight)->value;
  if (value == 0) {
    printf("Drive Light toggle\n");
    if (carState.Light == LIGHT::L2) {
      carState.Light = LIGHT::L1;
    } else {
      carState.Light = LIGHT::L2;
    }
  }
}

void nextScreenHandler() {
  if (carState.getPin(PinNextScreen)->value == 0) {
    switch (carState.displayStatus) {
    case DISPLAY_STATUS::ENGINEER_RUNNING:
      carState.displayStatus = DISPLAY_STATUS::DRIVER_SETUP;
      cout << "Switch Next Screen toggle: switch from eng --> driver" << endl;
      break;
    case DISPLAY_STATUS::DRIVER_RUNNING:
      carState.displayStatus = DISPLAY_STATUS::ENGINEER_SETUP;
      cout << "Switch Next Screen toggle: switch from driver --> eng" << endl;
      break;
    default:
      break;
    }
  }
}

void constantModeOnOffHandler() {
  if (carState.getPin(PinConstantModeOn)->value == 0) {
    if (carState.ConstantModeOn) {
      printf("ConstantMode OFF\n");
      carState.ConstantModeOn = false;
    } else {
      printf("ConstantMode ON\n");
      carState.ConstantModeOn = true;
    }
  }
}

void constantModeHandler() {
  if (carState.getPin(PinConstantMode)->value == 0) {
    printf("Constant mode toggle\n");
    if (carState.ConstantMode == CONSTANT_MODE::POWER) {
      carState.ConstantMode = CONSTANT_MODE::SPEED;
    } else {
      carState.ConstantMode = CONSTANT_MODE::POWER;
    }
  }
}
// end IO pin handler -----------------------------------------
#endif