//
// PCF8574 I/O Extension over I2C
//
#include <definitions.h>

#include <stdio.h>

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
extern DriverDisplay driverDisplay;
extern EngineerDisplay engineerDisplay;
extern bool systemOk;

#define DEBUGIOEXT

// ------------------
// FreeRTOS functions

void IOExt::re_init() { init(); }

void IOExt::init() {
  xSemaphoreTakeT(i2cBus.mutex);
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    printf("    Init IOExt %u...\n", devNr);
    if (ioExt.IOExtDevs[devNr].begin()) {
      printf("[x] Initialization of IOExt %u failed.\n", devNr);
    } else {
      char msg[100];
      for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
        int idx = devNr * 8 + pinNr;
        CarStatePin pin = carState.pins[idx];
        carState.idxOfPin.insert(pair<string, int>{pin.name, idx});
        ioExt.IOExtDevs[devNr].pinMode(pinNr, pin.mode);
        pin.value = pin.oldValue = 1;
        snprintf(msg, 100, "  0x%02x [%02d], mode:%s, value=%d (%s)\n", pin.port, carState.getIdx(pin.name),
                 pin.mode != OUTPUT ? "INPUT " : "OUTPUT", pin.value, pin.name.c_str());
        printf(msg);
      }
      snprintf(msg, 100, "[v] %s[%d] initialized.\n", getName().c_str(), devNr);
      printf(msg);
      driverDisplay.print(msg);
    }
    xSemaphoreGive(i2cBus.mutex);
  }
  readAll();
  set_SleepTime(100);
}

void IOExt::exit(void) {
  // TODO
}
// ------------------

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

void IOExt::handleIoInterrupt() {

  list<void (*)()> pinHandlerList;
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->mode != OUTPUT) {

        xSemaphoreTakeT(i2cBus.mutex);
        pin->value = ioExt.IOExtDevs[devNr].digitalRead(pinNr);
        xSemaphoreGive(i2cBus.mutex);

        // printf("0x%02x [%2d], %16s, %d ? %d ", pin->port, getIdx(pin->name), pin->name.c_str(), pin->value, pin->oldValue);
        if (pin->handlerFunction != NULL && (pin->value != pin->oldValue || !pin->inited)) {
          // printf("!!");
          pin->inited = true;
          pinHandlerList.push_back(pin->handlerFunction);
          pin->oldValue = pin->value;
        }
        // printf("\n");
      }
    }
  }

#ifdef DEBUGIOEXT
  printf("%s", carState.printIOs("").c_str());
#endif

  pinHandlerList.unique();
  for (void (*pinHandler)() : pinHandlerList) {
    pinHandler();
  }
  pinHandlerList.clear();
}

void IOExt::readAll() {

  // for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
  //   for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
  //     CarStatePin *pin = carState.getPin(devNr, pinNr);
  //     if (pin->mode != OUTPUT) {
  //       xSemaphoreTakeT(i2cBus.mutex);
  //       pin->value = ioExt.IOExtDevs[devNr].digitalRead(pinNr);
  //       xSemaphoreGive(i2cBus.mutex);
  //       if (pin->value != pin->oldValue || !pin->inited) {
  //         pin->oldValue = pin->value;
  //       }
  //     }
  //   }
  // }
  handleIoInterrupt();
#ifdef DEBUGIOEXT
  printf("%s", carState.printIOs("").c_str());
#endif
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

void ecoPowerHandler() { printf("EcoMowerMode %s\n", (carState.getPin(PinEcoPower)->value == 1 ? "Eco" : "Power")); }

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
      carState.Light=LIGHT::L1;
    } else {
      carState.Light=LIGHT::L2;
    }
  }
}

void nextScreenHandler() {
  if (carState.getPin(PinNextScreen)->value == 0) {
    printf("Switch Next Screen toggle\n");
    if (driverDisplay.get_DisplayStatus() == DISPLAY_STATUS::HALTED) {
      engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::HALTED);
      driverDisplay.set_DisplayStatus(DISPLAY_STATUS::SETUPDRIVER);
    } else {
      driverDisplay.set_DisplayStatus(DISPLAY_STATUS::HALTED);
      engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::SETUPENGINEER);
    }
  }
}

void constantModeOnOffHandler() {
  if (carState.getPin(PinConstantModeOn)->value == 0) {
    if (carState.ConstantModeOn) {
      printf("ConstantMode OFF\n");
      carState.ConstantModeOn=false;
    } else {
      printf("ConstantMode ON\n");
      carState.ConstantModeOn=true;
    }
  }
}

void constantModeHandler() {
  if (carState.getPin(PinConstantMode)->value == 0) {
    printf("Constant mode toggle\n");
    if (carState.ConstantMode == CONSTANT_MODE::POWER) {
      carState.ConstantMode=CONSTANT_MODE::SPEED;
    } else {
      carState.ConstantMode=CONSTANT_MODE::POWER;
    }
  }
}
// end IO pin handler -----------------------------------------

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

  xSemaphoreTakeT(i2cBus.mutex);
  ioExt.IOExtDevs[idx].digitalWrite(pin, value);
  xSemaphoreGive(i2cBus.mutex);
}

int IOExt::getPort(int port) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;
  int value = 0;
  xSemaphoreTakeT(i2cBus.mutex);
  value = ioExt.IOExtDevs[idx].digitalRead(pin);
  xSemaphoreGive(i2cBus.mutex);

  printf("0x%02x [%d|%d]: %d\n", port, idx, pin, value);
  return value;
}

void IOExt::getAll(CarStatePin *pins, int maxCount) {
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    printf("0x%02x: ", devNr);
    for (int pin = 0; pin < PCF8574_NUM_PORTS; pin++) {
      int id = (devNr << 4) + pin;
      if (pins[id].value != 0) {
        printf("%s: %d\n", pins[id].name.c_str(), pins[id].value);
      }
    }
    printf("\n");
  }
}

volatile bool IOExt::ioInterruptRequest = false;

void IOExt::task() {

  // polling loop
  while (1) {
    // handle input interrupts
    if (ioInterruptRequest && !isInInterruptHandler) {
      isInInterruptHandler = true;
      handleIoInterrupt();
      ioInterruptRequest = false;
      isInInterruptHandler = false;
    }

    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
