//
// PCF8574 I/O Extension over I2C
//

#include <stdio.h>

#include <PCF8574.h> // PCF8574
#include <Wire.h>    // I2C

#include <DriverDisplayC.h>
#include <I2CBus.h>
#include <IOExt.h>
#include <definitions.h>

extern I2CBus i2cBus;
extern Indicator indicator;
extern IOExt ioExt;
extern CarState carState;

#define DEBUGIOEXT

void IOExt::re_init() { init(); }

void IOExt::init() {

  // carState.pins = thePins;
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
        Pin pin = pins[idx];
        idxOfPin.insert(pair<string, int>{pin.name, idx});
        xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
        ioExt.IOExtDevs[devNr].pinMode(pinNr, pin.mode);
        xSemaphoreGive(i2cBus.mutex);
        pin.value = pin.oldValue = 1;
        sprintf(msg, "  0x%02x [%02d], mode:%s, value=%d (%s)\n", pin.port, getIdx(pin.name), pin.mode != OUTPUT ? "INPUT " : "OUTPUT",
                pin.value, pin.name.c_str());
        printf(msg);
        // DriverDisplayC::instance()->print(msg);
      }
      sprintf(msg, "[v] %s[%d] initialized.\n", getName().c_str(), devNr);
      printf(msg);
      DriverDisplayC::instance()->print(msg);
    }
  }
}

void IOExt::exit(void) {
  // TODO
}

Pin IOExt::pins[IOExtPINCOUNT] = { // IOExtDev0
    {0x00, INPUT_PULLUP, 1, 1, 0l, BatOnOff, batteryOnOffHandler},
    {0x01, INPUT_PULLUP, 1, 1, 0l, PvOnOff, pvOnOffHandler},
    {0x02, INPUT_PULLUP, 1, 1, 0l, McOnOff, mcOnOffHandler},
    {0x03, INPUT_PULLUP, 1, 1, 0l, EcoPower, ecoPowerHandler},
    {0x04, INPUT_PULLUP, 1, 1, 0l, FwdBwd, fwdBwdHandler},
    {0x05, INPUT_PULLUP, 1, 1, 0l, DUMMY06, NULL},
    {0x06, INPUT_PULLUP, 1, 1, 0l, DUMMY07, NULL},
    {0x07, OUTPUT, 1, 1, 0l, Relais11, NULL},
    // IOExtDev1
    {0x10, OUTPUT, 1, 1, 0l, Relais21, NULL},
    {0x11, OUTPUT, 1, 1, 0l, Relais22, NULL},
    {0x12, OUTPUT, 1, 1, 0l, Ralais12, NULL},
    {0x13, OUTPUT, 1, 1, 0l, Relais31, NULL},
    {0x14, OUTPUT, 1, 1, 0l, Relais32, NULL},
    {0x15, INPUT_PULLUP, 1, 1, 0l, BreakPedal, breakPedalHandler},
    {0x16, INPUT_PULLUP, 1, 1, 0l, DUMMY19, NULL},
    {0x17, INPUT_PULLUP, 1, 1, 0l, DUMMY17, NULL},
    // IOExtDev2
    {0x20, INPUT_PULLUP, 1, 1, 0l, IndicatorLeft, indicatorHandler},
    {0x21, INPUT_PULLUP, 1, 1, 0l, IndicatorRight, indicatorHandler},
    {0x22, INPUT_PULLUP, 1, 1, 0l, DriveLight, driveLightHandler},
    {0x23, INPUT_PULLUP, 1, 1, 0l, Light, lightHandler},
    {0x24, INPUT_PULLUP, 1, 1, 0l, Horn, hornHandler},
    {0x25, INPUT_PULLUP, 1, 1, 0l, NextScreen, nextScreenHandler},
    {0x26, INPUT_PULLUP, 1, 1, 0l, ConstantMode, constantModeHandler},
    {0x27, INPUT_PULLUP, 1, 1, 0l, ConstantSet, constantSetHandler},
    // IOExtDev3
    {0x30, INPUT_PULLUP, 1, 1, 0l, DUMMY31, NULL},
    {0x31, INPUT_PULLUP, 1, 1, 0l, Reserve1, NULL},
    {0x32, INPUT_PULLUP, 1, 1, 0l, DUMMY33, NULL},
    {0x33, INPUT_PULLUP, 1, 1, 0l, DUMMY34, NULL},
    {0x34, INPUT_PULLUP, 1, 1, 0l, DUMMY35, NULL},
    {0x35, INPUT_PULLUP, 1, 1, 0l, DUMMY36, NULL},
    {0x36, INPUT_PULLUP, 1, 1, 0l, DUMMY37, NULL},
    {0x37, INPUT_PULLUP, 1, 1, 0l, DUMMY38, NULL}};

void IOExt::handleIoInterrupt() {
  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);

#ifdef USE_LEGACY_PIN_READ
  // read all
  PCF8574::DigitalInput dra[PCF8574_NUM_DEVICES];
  for (int i = 0; i < PCF8574_NUM_DEVICES; i++) {
    dra[i] = IOExt[i].digitalReadAll();
  }
#else
  list<void (*)()> pinHandlerList;
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
      Pin *pin = ioExt.getPin(devNr, pinNr);
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
#endif
  xSemaphoreGive(i2cBus.mutex);

#ifdef DEBUGIOEXT
  for (int devNr = 0; devNr < PCF8574_NUM_DEVICES; devNr++) {
    printf("0x%2x0: ", devNr);
    for (int pinNr = 0; pinNr < PCF8574_NUM_PORTS; pinNr++) {
      string color = "";
      Pin *pin = ioExt.getPin(devNr, pinNr);
      if (pin->mode == OUTPUT) {
        printf(" \033[1;31m%d\033[0;39m", pin->value);
      } else {
        // printf(" %d.%d\033[0;39m", pin->value, pin->oldValue);
        printf(" %d", pin->value);
      }
      if ((getIdx(devNr, pinNr) + 1) % 8 == 0)
        printf(" | ");
      else if ((getIdx(devNr, pinNr) + 1) % 4 == 0)
        printf(" - ");
    }
  }
  printf("\n");
#endif

  pinHandlerList.unique();
  for (void (*pinHandler)() : pinHandlerList) {
    pinHandler();
  }
  pinHandlerList.clear();
}

int IOExt::getIdx(string pinName) { return idxOfPin.find(pinName)->second; }
Pin *IOExt::getPin(int devNr, int pinNr) { return &(ioExt.pins[getIdx(devNr, pinNr)]); }
Pin *IOExt::getPin(int port) { return &(ioExt.pins[getIdx(port)]); }
Pin *IOExt::getPin(string pinName) { return &(ioExt.pins[getIdx(pinName)]); }

// IO pin handler -----------------------------------------
void batteryOnOffHandler() { printf("Battery %s\n", (ioExt.getPin(BatOnOff)->value == 1 ? "On" : "Off")); }
void pvOnOffHandler() { printf("PV %s\n", (ioExt.getPin(PvOnOff)->value == 1 ? "On" : "Off")); }
void mcOnOffHandler() { printf("MC %s\n", (ioExt.getPin(McOnOff)->value == 1 ? "On" : "Off")); }
void ecoPowerHandler() { printf("EcoMowerMode %s\n", (ioExt.getPin(EcoPower)->value == 1 ? "Eco" : "Power")); }
void fwdBwdHandler() { printf("Direction %s\n", (ioExt.getPin(FwdBwd)->value == 1 ? "Forward" : "Backward")); }
void breakPedalHandler() { printf("Break pedal pressed %s\n", (ioExt.getPin(BreakPedal)->value == 1 ? "yes" : "no")); }
void indicatorHandler() {
  int indiLeft = ioExt.getPin(IndicatorLeft)->value;
  int indiRight = ioExt.getPin(IndicatorRight)->value;
  if (indiLeft == 0 || indiRight == 0) {
    // printf("Indicator\n");
    indicator.setIndicator(indiLeft, indiRight);
  }
}
void hornHandler() {
  int value = ioExt.getPin(Horn)->value;
  printf("Horn %s\n", (value == 0 ? "On" : "Off"));
}
void lightHandler() {
  int value = ioExt.getPin(Light)->value;
  if (value == 0) {
    printf("Light toggle\n");
    DriverDisplayC::instance()->light1OnOff();
  }
}
void driveLightHandler() {
  int value = ioExt.getPin(DriveLight)->value;
  if (value == 0) {
    printf("Drive Light toggle\n");
    DriverDisplayC::instance()->light2OnOff();
  }
}
void nextScreenHandler() {
  int value = ioExt.getPin(NextScreen)->value;
  if (value == 0) {
    printf("Switch Next Screen toggle\n");
  }
}
void constantSetHandler() {
  int value = ioExt.getPin(ConstantSet)->value;
  if (value == 0) {
    printf("Constant set toggle\n");
    DriverDisplayC::instance()->constant_drive_mode_set(CONSTANT_MODE::POWER);
  }
}
void constantModeHandler() {
  int value = ioExt.getPin(ConstantMode)->value;
  if (value == 0) {
    printf("Constant mode toggle\n");
    DriverDisplayC::instance()->constant_drive_mode_show();
  }
}
// end IO pin handler -----------------------------------------

void IOExt::setMode(int port, uint8_t mode) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  ioExt.IOExtDevs[idx].pinMode(pin, mode);
  xSemaphoreGive(i2cBus.mutex);
}

void IOExt::set(int port, bool value) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  ioExt.IOExtDevs[idx].digitalWrite(pin, value);
  xSemaphoreGive(i2cBus.mutex);
}

int IOExt::get(int port) {
  // get device & port
  int idx = port >> 4;
  int pin = port & 0xf;

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  int value = ioExt.IOExtDevs[idx].digitalRead(pin);
  xSemaphoreGive(i2cBus.mutex);

  printf("0x%02x [%d|%d]: %d\n", port, idx, pin, value);
  return value;
}

void IOExt::getAll(Pin *pins, int maxCount) {
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
