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
#include <CarControl.h>
#include <definitions.h>

extern I2CBus i2cBus;
extern Indicator indicator;
extern IOExt ioExt;
extern CarState carState;
extern DriverDisplay driverDisplay;
extern EngineerDisplay engineerDisplay;

void CarControl::re_init() { init(); }

void CarControl::init() {

  char msg[100];

  xSemaphoreTake(i2cBus.mutex, portMAX_DELAY);
  xSemaphoreGive(i2cBus.mutex);
}

void CarControl::exit(void) {
  // TODO
}


void CarControl::handleValueChanged() {
  //TODO
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
volatile int CarControl::valueChangeRequest = 0;

void CarControl::task() {
  // polling loop
  while (1) {
    // handle input interrupts
    if (valueChangeRequest > 0 && !isInValueChangedHandler) {
      isInValueChangedHandler = true;
      handleValueChanged();
      valueChangeRequest = 0;
      isInValueChangedHandler = false;
    }
    // sleep
    vTaskDelay(taskSleep / portTICK_PERIOD_MS);
  }
}
