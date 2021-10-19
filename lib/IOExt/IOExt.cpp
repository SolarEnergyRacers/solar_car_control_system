//
// PCF8574 I/O Extension over I2C
//

#include <stdio.h>

#include <PCF8574.h> // PCF8574
#include <Wire.h>    // I2C

#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <I2CBus.h>
#include <IOExt.h>
#include <definitions.h>

extern I2CBus i2cBus;
extern Indicator indicator;
extern IOExt ioExt;
extern CarState carState;
extern DriverDisplay driverDisplay;
extern EngineerDisplay engineerDisplay;

#define DEBUGIOEXT

void IOExt::re_init() { init(); }

void IOExt::init() {

  char msg[100];

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    printf("    Init IOExt %u...\n", devNr);
    if (ioExt.IOExtDevs[devNr].begin()) {
      xSemaphoreGive(i2cBus.mutex);
      printf("[x] Initialization of IOExt %u failed.\n", devNr);
    } else {
      xSemaphoreGive(i2cBus.mutex);
      for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
        int idx = devNr * 8 + pinNr;
        CarStatePin pin = carState.pins[idx];
        carState.idxOfPin.insert(pair<string, int>{pin.name, idx});
        xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
        ioExt.IOExtDevs[devNr].pinMode(pinNr, pin.mode);
        xSemaphoreGive(i2cBus.mutex);
        pin.value = pin.oldValue = 1;
        sprintf(msg, "  0x%02x [%02d], mode:%s, value=%d (%s)\n", pin.port, carState.getIdx(pin.name),
                pin.mode != OUTPUT ? "INPUT " : "OUTPUT", pin.value, pin.name.c_str());
        printf(msg);
      }
      sprintf(msg, "[v] %s[%d] initialized.\n", getName().c_str(), devNr);
      printf(msg);
      driverDisplay.print(msg);
    }
  }
  readAll();
}

void IOExt::exit(void) {
  // TODO
}

CarStatePin CarState::pins[] = { // IOExtDev0
    {0x00, INPUT_PULLUP, 1, 1, 0l, PinBatOnOff, batteryOnOffHandler},
    {0x01, INPUT_PULLUP, 1, 1, 0l, PinPvOnOff, pvOnOffHandler},
    {0x02, INPUT_PULLUP, 1, 1, 0l, PinMcOnOff, mcOnOffHandler},
    {0x03, INPUT_PULLUP, 1, 1, 0l, PinEcoPower, ecoPowerHandler},
    {0x04, INPUT_PULLUP, 1, 1, 0l, PinDUMMY06, NULL},
    {0x05, INPUT_PULLUP, 1, 1, 0l, PinFwdBwd, fwdBwdHandler},
    {0x06, INPUT_PULLUP, 1, 1, 0l, PinDUMMY07, NULL},
    {0x07, OUTPUT, 1, 1, 0l, PinRelais11, NULL},
    // IOExtDev1
    {0x10, OUTPUT, 1, 1, 0l, PinRelais21, NULL},
    {0x11, OUTPUT, 1, 1, 0l, PinRelais22, NULL},
    {0x12, OUTPUT, 1, 1, 0l, PinRalais12, NULL},
    {0x13, OUTPUT, 1, 1, 0l, PinRelais31, NULL},
    {0x14, OUTPUT, 1, 1, 0l, PinRelais32, NULL},
    {0x15, INPUT_PULLUP, 1, 1, 0l, PinBreakPedal, breakPedalHandler},
    {0x16, INPUT_PULLUP, 1, 1, 0l, PinDUMMY19, NULL},
    {0x17, INPUT_PULLUP, 1, 1, 0l, PinDUMMY17, NULL},
    // IOExtDev2
    {0x20, INPUT_PULLUP, 1, 1, 0l, PinIndicatorLeft, indicatorHandler},
    {0x21, INPUT_PULLUP, 1, 1, 0l, PinIndicatorRight, indicatorHandler},
    {0x22, INPUT_PULLUP, 1, 1, 0l, PinHeadLight, headLightHandler},
    {0x23, INPUT_PULLUP, 1, 1, 0l, PinLight, lightHandler},
    {0x24, INPUT_PULLUP, 1, 1, 0l, PinHorn, hornHandler},
    {0x25, INPUT_PULLUP, 1, 1, 0l, PinNextScreen, nextScreenHandler},
    {0x26, INPUT_PULLUP, 1, 1, 0l, PinConstantMode, constantModeHandler},
    {0x27, INPUT_PULLUP, 1, 1, 0l, PinConstantSet, constantSetHandler},
    // IOExtDev3
    {0x30, INPUT_PULLUP, 1, 1, 0l, PinDUMMY31, NULL},
    {0x31, INPUT_PULLUP, 1, 1, 0l, PinReserve1, NULL},
    {0x32, INPUT_PULLUP, 1, 1, 0l, PinDUMMY33, NULL},
    {0x33, INPUT_PULLUP, 1, 1, 0l, PinDUMMY34, NULL},
    {0x34, INPUT_PULLUP, 1, 1, 0l, PinDUMMY35, NULL},
    {0x35, INPUT_PULLUP, 1, 1, 0l, PinDUMMY36, NULL},
    {0x36, INPUT_PULLUP, 1, 1, 0l, PinDUMMY37, NULL},
    {0x37, INPUT_PULLUP, 1, 1, 0l, PinDUMMY38, NULL}};

void IOExt::handleIoInterrupt() {
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  list<void (*)()> pinHandlerList;
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->mode != OUTPUT) {
        pin->value = ioExt.IOExtDevs[devNr].digitalRead(pinNr);
        // printf("0x%02x [%2d], %16s, %d ? %d ", pin->port, getIdx(pin->name), pin->name.c_str(), pin->value, pin->oldValue);
        if (pin->handlerFunction != NULL && pin->value != pin->oldValue) {
          // printf("!!");
          pinHandlerList.push_back(pin->handlerFunction);
          pin->oldValue = pin->value;
        }
        // printf("\n");
      }
    }
  }
  xSemaphoreGive(i2cBus.mutex);

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
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
      CarStatePin *pin = carState.getPin(devNr, pinNr);
      if (pin->mode != OUTPUT) {
        pin->value = ioExt.IOExtDevs[devNr].digitalRead(pinNr);
        if (pin->value != pin->oldValue) {
          pin->oldValue = pin->value;
        }
      }
    }
  }
#ifdef DEBUGIOEXT
  printf("%s", carState.printIOs("").c_str());
#endif
  xSemaphoreGive(i2cBus.mutex);
}

// IO pin handler -----------------------------------------
void batteryOnOffHandler() { printf("Battery %s\n", (carState.getPin(PinBatOnOff)->value == 1 ? "On" : "Off")); }
void pvOnOffHandler() { printf("PV %s\n", (carState.getPin(PinPvOnOff)->value == 1 ? "On" : "Off")); }
void mcOnOffHandler() { printf("MC %s\n", (carState.getPin(PinMcOnOff)->value == 1 ? "On" : "Off")); }
void ecoPowerHandler() { printf("EcoMowerMode %s\n", (carState.getPin(PinEcoPower)->value == 1 ? "Eco" : "Power")); }
void fwdBwdHandler() {
  printf("Direction %s\n", (carState.getPin(PinFwdBwd)->value == 1 ? "Forward" : "Backward"));
  carState.DriveDirection.set(carState.getPin(PinFwdBwd)->value == 1 ? DRIVE_DIRECTION::FORWARD : DRIVE_DIRECTION::BACKWARD);
  driverDisplay.write_drive_direction(carState.DriveDirection.get());
}
void breakPedalHandler() { printf("Break pedal pressed %s\n", (carState.getPin(PinBreakPedal)->value == 1 ? "yes" : "no")); }
void indicatorHandler() {
  int indiLeft = carState.getPin(PinIndicatorLeft)->value;
  int indiRight = carState.getPin(PinIndicatorRight)->value;
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
    int devNr = 1;
    int pinNr = 3;
    if (carState.Light.get() == LIGHT::L1) {
      carState.Light.set(LIGHT::OFF);
      ioExt.setPort((devNr << 4) + pinNr, false);
    } else {
      carState.Light.set(LIGHT::L1);
      ioExt.setPort((devNr << 4) + pinNr, true);
    }
    driverDisplay.show_light();
  }
}
void headLightHandler() {
  int value = carState.getPin(PinHeadLight)->value;
  if (value == 0) {
    printf("Drive Light toggle\n");
    if (carState.Light.get() == LIGHT::L2) {
      carState.Light.set(LIGHT::L1);
    } else {
      carState.Light.set(LIGHT::L2);
    }
    driverDisplay.show_light();
  }
}
void nextScreenHandler() {
  int value = carState.getPin(PinNextScreen)->value;
  if (value == 0) {
    printf("Switch Next Screen toggle\n");
    if (driverDisplay.get_DisplayStatus() == DISPLAY_STATUS::ENGINEER) {
      engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::HALTED);
      driverDisplay.set_DisplayStatus(DISPLAY_STATUS::SETUPDRIVER);
    } else {
      driverDisplay.set_DisplayStatus(DISPLAY_STATUS::HALTED);
      engineerDisplay.set_DisplayStatus(DISPLAY_STATUS::SETUPENGINEER);
    }
  }
}
void constantSetHandler() {
  int value = carState.getPin(PinConstantSet)->value;
  if (value == 0) {
    printf("Constant set toggle\n");
    if (carState.ConstantModeOn.get()) {
      carState.ConstantModeOn.set(false);
    } else {
      carState.ConstantModeOn.set(true);
    }
    driverDisplay.constant_drive_mode_show();
  }
}
void constantModeHandler() {
  int value = carState.getPin(PinConstantMode)->value;
  if (value == 0) {
    printf("Constant mode toggle\n");
    if (carState.ConstantMode.get() == CONSTANT_MODE::POWER) {
      carState.ConstantMode.set(CONSTANT_MODE::SPEED);
    } else {
      carState.ConstantMode.set(CONSTANT_MODE::POWER);
    }
    driverDisplay.constant_drive_mode_show();
  }
}
// end IO pin handler -----------------------------------------

void IOExt::setPortMode(int port, uint8_t mode) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  ioExt.IOExtDevs[idx].pinMode(pin, mode);
  xSemaphoreGive(i2cBus.mutex);
}

void IOExt::setPort(int port, bool value) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  ioExt.IOExtDevs[idx].digitalWrite(pin, value);
  xSemaphoreGive(i2cBus.mutex);
}

int IOExt::getPort(int port) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  int value = ioExt.IOExtDevs[idx].digitalRead(pin);
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
    vTaskDelay(taskSleep / portTICK_PERIOD_MS);
  }
}
