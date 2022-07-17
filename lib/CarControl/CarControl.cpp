//
// MCP23017 I/O Extension over I2C
//
#include <definitions.h>

#include <fmt/core.h>
#include <fmt/printf.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <Wire.h> // I2C

#include <ADC.h>
#include <CarControl.h>
#include <CarSpeed.h>
#include <ConfigFile.h>
#include <Console.h>
#include <DAC.h>
#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
#include <I2CBus.h>
#include <IOExt.h>
#include <Indicator.h>
#include <MCP23017.h>
#include <SDCard.h>

#if ADC_ON
extern ADC adc;
#endif
extern CarControl carControl;
extern CarSpeed carSpeed;
extern CarState carState;
extern Console console;
#if DAC_ON
extern DAC dac;
#endif
extern DriverDisplay driverDisplay;
extern EngineerDisplay engineerDisplay;
extern I2CBus i2cBus;
extern Indicator indicator;
extern IOExt ioExt;
extern SDCard sdCard;

using namespace std;

unsigned long millisNextStamp = millis();

// ------------------
// FreeRTOS functions

string CarControl::re_init() { return init(); }

string CarControl::init() {
  bool hasError = false;
  console << "[  ] Init 'CarControl'... ";
  justInited = true;
  // mutex = xSemaphoreCreateMutex();
  // xSemaphoreGive(mutex);
  carState.AccelerationDisplay = -99;
  // adjust_paddles(carState.PaddleAdjustCounter); // manually adjust paddles (5s handling time)
  console << "done.\n";
  return fmt::format("[{}] CarControl initialized.", hasError ? "--" : "ok");
}

void CarControl::exit(void) {
  // TODO
}
// --------------------

void CarControl::_handleValueChanged() {
  // TODO
}

bool CarControl::read_battery_data() {
#if ADC_ON
  carState.BatteryVoltage = adc.read(ADC::Pin::BAT_VOLTAGE) / 100.;  // TODO
  carState.BatteryCurrent = adc.read(ADC::Pin::BAT_CURRENT) / 1000.; // TODO
#endif
  return true;
}

bool CarControl::read_motor_data() {
#if ADC_ON
  carState.MotorVoltage = adc.read(ADC::Pin::MOTOR_VOLTAGE) / 100.;  // TODO
  carState.MotorCurrent = adc.read(ADC::Pin::MOTOR_CURRENT) / 1000.; // TODO
#endif
  return true;
}

bool CarControl::read_pv_data() {
#if ADC_ON
  carState.PhotoVoltaicCurrent = adc.read(ADC::Pin::PV_CURRENT) / 100.; // TODO
#endif
  return true;
}

bool CarControl::read_reference_cell_data() {
#if ADC_ON
  carState.ReferenceSolarCell = adc.read(ADC::Pin::REFERENCE_CELL);
#endif
  return true;
}

bool CarControl::read_speed() {
#if ADC_ON
  // native input
  int16_t value = adc.read(ADC::Pin::MOTOR_SPEED);
  // voltage
  float voltage = value * adc.get_multiplier(ADC::Pin::MOTOR_SPEED);
  // round per minute
  float rpm = 370 * voltage;
  // speed
  float radius = 0.50; // m
  float speed = 3.1415 * radius * rpm / 60 * 3.6;
  carState.Speed = (int)speed;
  // console << fmt::sprintf("raw %5d | %5.2f, rpm:%5.2f, speed:%5.2f, %4d\n", value, voltage, rpm, speed, (int)speed);
#endif
  return true;
}

bool CarControl::read_paddles() {
  bool hasChanged = false;
  if (carState.BreakPedal) {
    // XXXXXX
    // console << "Break Pedal Pressed (paddle control)\n";
    _set_dec_acc_values(DAC_MAX, 0, ADC_MAX, 0, -88);
    return true;
  }
  int16_t valueDec = carState.STW_DEC; // adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = carState.STW_ACC; // adc.read(ADC::Pin::STW_ACC);
  // check if change is in damping
  if (valueAcc != 0 && valueDec != 0)
    if (abs(accelLast - valueAcc) < carState.PaddleDamping && abs(recupLast - valueDec) < carState.PaddleDamping)
      return hasChanged;

  int16_t valueDecNorm = 0;
  int16_t valueAccNorm = 0;
  int valueDisplay = 0;

  valueDecNorm = CarControl::_normalize(0, MIN_DISPLAY_VALUE, ads_min_dec, ads_max_dec, valueDec);
  if (valueDecNorm > 0) {
    valueDisplay = -valueDecNorm;
  } else {
    valueAccNorm = CarControl::_normalize(0, MAX_DISPLAY_VALUE, ads_min_acc, ads_max_acc, valueAcc);
    if (valueAccNorm > 0) {
      valueDisplay = valueAccNorm;
    } else {
      valueDisplay = 0;
    }
  }
  // prepare and write motor acceleration and recuperation values to DigiPot
  int valueDecPot = 0;
  int valueAccPot = 0;
  if (valueDisplay < 0) {
    valueDecPot = -(int)(((float)valueDisplay / MAX_DISPLAY_VALUE) * DAC_MAX);
    valueAccPot = 0;
  } else {
    valueDecPot = 0;
    valueAccPot = (int)(((float)valueDisplay / MAX_DISPLAY_VALUE) * DAC_MAX);
  }

  if (valueDisplayLast != valueDisplay) {
    // debug_printf("Dec (v0):  %5d --> %3d | Acc (v1): %5d --> %3d | ACCEL-DISPLAY: %3d"
    //              " ==> set POT0 =%3d (dec(%5d-%5d)), POT1 =%3d (acc(%5d-%5d))\n",
    //              valueDec, valueDecNorm, valueAcc, valueAccNorm, valueDisplay, valueDecPot, ads_min_dec, ads_max_dec, valueAccPot,
    //              ads_min_acc, ads_max_acc);
    hasChanged = true;
    _set_dec_acc_values(valueDecPot, valueAccPot, valueDec, valueAcc, valueDisplay);
  }

  return hasChanged;
}

void CarControl::_set_dec_acc_values(int valueDecPot, int valueAccPot, int16_t valueDec, int16_t valueAcc, int valueDisplay) {
// debug_printf("Dec (v0):  %5d  | Acc (v1): %5d  | ACCEL-DISPLAY: %3d ==> set POT0 =%3d (dec(%5d-%5d)), POT1 =%3d (acc(%5d-%5d))\n",
//              valueDec, valueAcc, valueDisplay, valueDecPot, ads_min_dec, ads_max_dec, valueAccPot, ads_min_acc, ads_max_acc);
#if DAC_ON
  dac.set_pot(valueDecPot, DAC::pot_chan::POT_CHAN1);
  dac.set_pot(valueAccPot, DAC::pot_chan::POT_CHAN0);
#endif
  carState.Deceleration = valueDec;
  carState.Acceleration = valueAcc;
  carState.AccelerationDisplay = valueDisplay;

  valueDisplayLast = valueDisplay;
}

void CarControl::adjust_paddles(int cycles) {
  int x, y;

#if DAC_ON
  dac.reset_and_lock_pot();
#endif
  ads_min_acc = 50000;
  ads_min_dec = 50000;
  ads_max_acc = 0;
  ads_max_dec = 0;

  if (cycles < 1)
    cycles = 1;
  string s("    adjust...");
  console << s;
  if (engineerDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
    carState.DriverInfo = s;
  } else {
    engineerDisplay.print(s.c_str());
    engineerDisplay.getCursor(x, y);
  }
  while (cycles-- > 0) {
    s = fmt::format(" paddle adjust:\n {:2d}", cycles);
    console << s;
    if (engineerDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
      carState.DriverInfo = s;
    } else {
      engineerDisplay.write_ganz_99(x, y, cycles + 1, cycles, 1, false, ILI9341_BLUE, ILI9341_ORANGE);
    }

#if ADC_ON
    int16_t value;
    value = adc.read(ADC::Pin::STW_DEC);
    if (value > 0) {
      if (ads_min_dec > value)
        ads_min_dec = value;
      if (ads_max_dec < value)
        ads_max_dec = value;
    }
    value = adc.read(ADC::Pin::STW_ACC);
    if (value > 0) {
      if (ads_min_acc > value)
        ads_min_acc = value;
      if (ads_max_acc < value)
        ads_max_acc = value;
    }
#endif
    delay(100);
  }
  // make sure null level to avoid automatic acceleration/deceleration
  ads_min_dec += carState.PaddleOffset;
  ads_min_acc += carState.PaddleOffset;

  s = fmt::format("\n    ==>dec {:5}-{:5} == acc {:5}-{:5}\n", ads_min_dec, ads_max_dec, ads_min_acc, ads_max_acc);
  console << s;
  if (engineerDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
    carState.DriverInfo = fmt::format("==> dec {:5}-{:5}       ==> acc {:5}-{:5}", ads_min_dec, ads_max_dec, ads_min_acc, ads_max_acc);
  } else {
    engineerDisplay.print(s.c_str());
  }
  //#SAFTY#
  carState.PaddlesJustAdjusted = true;
}

int CarControl::_normalize(int minDisplayValue, int maxDisplayValue, int minValue, int maxValue, int value) {
  float displArea = maxDisplayValue - minDisplayValue;
  float valueArea = maxValue - minValue;

  float retValue = (value - minValue) * displArea / valueArea;

  return (int)retValue;
}

void CarControl::_handle_indicator() {
  // if (valueChangeRequest) {
  if (carState.Indicator == INDICATOR::RIGHT) {
    carState.getPin(PinIndicatorOutLeft)->value = 0;
    carState.getPin(PinIndicatorOutRight)->value = 1;
  } else if (carState.Indicator == INDICATOR::LEFT) {
    carState.getPin(PinIndicatorOutLeft)->value = 1;
    carState.getPin(PinIndicatorOutRight)->value = 0;
  } else if (carState.Indicator == INDICATOR::WARN) {
    carState.getPin(PinIndicatorOutRight)->value = 1;
    carState.getPin(PinIndicatorOutLeft)->value = 1;
  } else {
    // INDICATOR::OFF
    carState.getPin(PinIndicatorOutLeft)->value = 0;
    carState.getPin(PinIndicatorOutRight)->value = 0;
  }
  //}
}

volatile int CarControl::valueChangeRequest = 0;

void CarControl::task() {
  // polling loop
  while (1) {
    carState.getPin(PinDacLifeSign)->value = !carState.getPin(PinDacLifeSign)->value;
    bool someThingChanged = false;
    someThingChanged |= read_paddles();
    someThingChanged |= read_motor_data();
    someThingChanged |= read_battery_data();
    someThingChanged |= read_speed();
    someThingChanged |= read_reference_cell_data();

    _handle_indicator();

    // one data row per second
    if (sdCard.isReadyForLog() && (millis() > millisNextStamp)) {
      millisNextStamp = millis() + carState.CarDataLogPeriod;
      sdCard.write(carState.csv("Recent State"));
    }

    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
