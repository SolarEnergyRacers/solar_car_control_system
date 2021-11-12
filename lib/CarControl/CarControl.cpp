//
// PCF8574 I/O Extension over I2C
//
#include <definitions.h>

#include <stdio.h>

#include <PCF8574.h> // PCF8574
#include <Wire.h>    // I2C

#include <ADC.h>
#include <CarControl.h>
#include <DAC.h>
#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
#include <I2CBus.h>
#include <IOExt.h>

extern I2CBus i2cBus;
extern Indicator indicator;
extern IOExt ioExt;
extern ADC adc;
extern DAC dac;
extern CarState carState;
extern CarControl carControl;
extern DriverDisplay driverDisplay;
extern EngineerDisplay engineerDisplay;

// ------------------
// FreeRTOS functions

void CarControl::re_init() { init(); }

void CarControl::init() {
  printf("[?] Setup '%s'...\n", getName().c_str());
  justInited = true;
  mutex = xSemaphoreCreateMutex();
  xSemaphoreGive(mutex);
  sleep_polling_ms = 250;
  printf("[v] %s inited.\n", getName().c_str());
  driverDisplay.print("[v] " + getName() + " initialized.\n");
}

void CarControl::exit(void) {
  // TODO
}

void CarControl::_handleValueChanged() {
  // TODO
}
// ------------------

bool CarControl::read_battery_data() {
  carState.BatteryVoltage=adc.read(ADC::Pin::BAT_VOLTAGE) / 100.;  // TODO
  carState.BatteryCurrent=adc.read(ADC::Pin::BAT_CURRENT) / 1000.; // TODO
  return true;
}

bool CarControl::read_motor_data() {
  carState.MotorVoltage=adc.read(ADC::Pin::MOTOR_VOLTAGE) / 100.;  // TODO
  carState.MotorCurrent=adc.read(ADC::Pin::MOTOR_CURRENT) / 1000.; // TODO
  return true;
}

bool CarControl::read_pv_data() {
  carState.PhotoVoltaicCurrent=adc.read(ADC::Pin::PV_CURRENT) / 100.; // TODO
  return true;
}

bool CarControl::read_speed() {
  // native input
  // xSemaphoreTakeT(carControl.mutex);
  int16_t value = adc.read(ADC::Pin::MOTOR_SPEED);
  // xSemaphoreGive(carControl.mutex);
  // voltage
  float voltage = value * adc.get_multiplier(ADC::Pin::MOTOR_SPEED);
  // round per minute
  float rpm = 370 * voltage;
  // speed
  float radius = 0.50; // m
  float speed = 3.1415 * radius * rpm / 60 * 3.6;
  carState.Speed=(int)speed;
  debug_printf_l3("raw %5d | %5.2f, rpm:%5.2f, speed:%5.2f, %4d\n", value, voltage, rpm, speed, (int)speed);
  return true;
}

bool CarControl::read_paddles() {
  int16_t valueDec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);

  int16_t valueAccNorm = 0;
  int16_t valueDecNorm = 0;
  int16_t valueDisplay = 0;

  if (justInited) {
    adjust_paddles_min();
    justInited = false;
  }

  if (valueAcc < 0) {
    // filter artefacts
    valueAcc = ads_min_acc;
  }
  if (valueDec < 0) {
    // filter artefacts
    valueDec = ads_min_dec;
  }

  if (valueDec < ads_min_dec || valueDec > ads_max_dec) {
    // filter artefacts
    valueDec = recupLast;
  }

  if (valueAcc < ads_min_acc || valueAcc > ads_max_acc) {
    // filter artefacts
    valueAcc = accelLast;
  }

  // readjust paddle area
  if (ads_min_dec > valueDec) {
    debug_printf("Adopt ads_min_dec (%5d) to valueDec (%5d - %d)\n", ads_min_dec, valueDec, MIN_ADJUST_GAP);
    ads_min_dec = valueDec - MIN_ADJUST_GAP;
  }
  if (ads_max_dec < valueDec) {
    debug_printf("Adopt ads_max_dec (%5d) to valueDec (%5d + %d)\n", ads_max_dec, valueDec, MAX_ADJUST_GAP);
    ads_max_dec = valueDec + MAX_ADJUST_GAP;
  }
  if (ads_min_acc > valueAcc) {
    debug_printf("Adopt ads_min_acc (%5d) to valueDec (%5d - %d)\n", ads_min_acc, valueAcc, MIN_ADJUST_GAP);
    ads_min_acc = valueAcc - MIN_ADJUST_GAP;
  }
  if (ads_max_acc < valueAcc) {
    debug_printf("Adopt ads_max_acc (%5d) to valueAcc (%5d + %d)\n", ads_max_acc, valueAcc, MAX_ADJUST_GAP);
    ads_max_acc = valueAcc + MAX_ADJUST_GAP;
  }

  valueDecNorm = CarControl::_normalize(valueDec, MAX_DISPLAY_VALUE, ads_min_dec, ads_max_dec);
  if (valueDecNorm > 0) {
    valueAccNorm = 0;
    valueDisplay = -valueDecNorm;
  } else {
    valueAccNorm = CarControl::_normalize(valueAcc, MAX_DISPLAY_VALUE, ads_min_acc, ads_max_acc);
    valueDisplay = valueAccNorm;
  }
  valueDisplay = (int)((valueDisplayLast * (SMOOTHING - 1) + valueDisplay) / SMOOTHING);

  // prepare and write motor acceleration and recuperation values to DigiPot
  // int valueDecPot = 0;
  // int valueAccPot = 0;
  // if (valueDisplay < 0) {
  //   valueDecPot = -(int)(((float)valueDisplay / MAX_DISPLAY_VALUE) * 1024);
  // } else {
  //   valueAccPot = (int)(((float)valueDisplay / MAX_DISPLAY_VALUE) * 1024);
  // }

  // if (valueDisplayLast != valueDisplay) {
  //   debug_printf("Dec (v0):  %5d --> %3d | Acc (v1): %5d --> %3d | "
  //                "ACCEL-DISPLAY: %3d"
  //                " ==> set POT0 =%3d (dec(%5d-%5d)), POT1 =%3d (acc(%5d-%5d))\n",
  //                valueDec, valueDecNorm, valueAcc, valueAccNorm, valueDisplay, valueDecPot, ads_min_dec, ads_max_dec, valueAccPot,
  //                ads_min_acc, ads_max_acc);

  //   valueDisplayLast = valueDisplay;
  //   dac.set_pot(valueAccPot, DAC::pot_chan::POT_CHAN0);
  //   dac.set_pot(valueDecPot, DAC::pot_chan::POT_CHAN1);
  // }

  carState.Acceleration=valueAcc;
  carState.Deceleration=valueDec;
  carState.AccelerationDisplay=valueDisplay;

  //  return valueDisplayLast;
  return true;
}

void CarControl::adjust_paddles_min() {
  int16_t valueDec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);
  // filter artefacts
  ads_min_dec = valueDec <= MIN_ADJUST_GAP ? 0 : valueDec - MIN_ADJUST_GAP;
  ads_min_acc = valueAcc <= MIN_ADJUST_GAP ? 0 : valueAcc - MIN_ADJUST_GAP;
  valueDisplayLast = 0;
  debug_printf("Set MIN values set dec: %d-->%d, acc: %d-->%d\n", valueDec, ads_min_dec, valueAcc, ads_min_acc);
}

void CarControl::adjust_paddles_max() {
  int16_t valueDec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);
  // filter artefacts
  ads_max_dec = valueDec >= 32767 + MAX_ADJUST_GAP ? 32767 : valueDec + MAX_ADJUST_GAP;
  ads_max_acc = valueAcc >= 32767 + MAX_ADJUST_GAP ? 32767 : valueAcc + MAX_ADJUST_GAP;
  valueDisplayLast = 0;
  debug_printf("Set MAX values dec: %d-->%d, acc: %d-->%d\n", valueDec, ads_min_dec, valueAcc, ads_min_acc);
}

int CarControl::_normalize(int value, int maxValue, int min, int max) {
  int delta = max - min;
  int val0based = value - min;
  int retValue = (int)(val0based * maxValue / delta);
  if (abs(retValue) < (int)(MAX_DISPLAY_VALUE / 10)) {
    retValue = 0;
  }
  return retValue;
}

// IO pin handler -----------------------------------------
// void batteryOnOffHandler() { printf("Battery %s\n", (carState.getPin(PinBatOnOff)->value == 1 ? "On" : "Off")); }
// void pvOnOffHandler() { printf("PV %s\n", (carState.getPin(PinPvOnOff)->value == 1 ? "On" : "Off")); }
// void mcOnOffHandler() { printf("MC %s\n", (carState.getPin(PinMcOnOff)->value == 1 ? "On" : "Off")); }
// void ecoPowerHandler() { printf("EcoMowerMode %s\n", (carState.getPin(PinEcoPower)->value == 1 ? "Eco" : "Power")); }
// void fwdBwdHandler() {
//   printf("Direction %s\n", (carState.getPin(PinFwdBwd)->value == 1 ? "Forward" : "Backward"));
//   carState.DriveDirection=carState.getPin(PinFwdBwd)->value == 1 ? DRIVE_DIRECTION::FORWARD : DRIVE_DIRECTION::BACKWARD;
//   driverDisplay.write_drive_direction();
// }
// void breakPedalHandler() { printf("Break pedal pressed %s\n", (carState.getPin(PinBreakPedal)->value == 1 ? "yes" : "no")); }
// void indicatorHandler() {
//   int indiLeft = carState.getPin(PinIndicatorBtnLeft)->value;
//   int indiRight = carState.getPin(PinIndicatorBtnRight)->value;
//   if (indiLeft == 0 || indiRight == 0) {
//     indicator.setIndicator(indiLeft, indiRight);
//   }
// }

void CarControl::_handle_indicator() {
  // ioExt.setPort(carState.getPin(PinIndicatorOutLeft)->port, indicator.getIndicatorLeft());
  // ioExt.setPort(carState.getPin(PinIndicatorOutRight)->port, indicator.getIndicatorRight());
}

volatile int CarControl::valueChangeRequest = 0;

// int lastValue = 0;
void CarControl::task() {
  // polling loop
  while (1) {
    bool someThingChanged = false;
    someThingChanged |= read_paddles();
    someThingChanged |= read_motor_data();
    someThingChanged |= read_battery_data();
    someThingChanged |= read_pv_data();
    someThingChanged |= read_speed();

    _handle_indicator();

    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
