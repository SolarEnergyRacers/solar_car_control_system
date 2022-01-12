//
// MCP23017 I/O Extension over I2C
//
#include <definitions.h>

#include <fmt/core.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <Wire.h> // I2C

#include <ADC.h>
#include <CarControl.h>
#include <DAC.h>
#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
#include <I2CBus.h>
#include <IOExt2.h>
#include <Indicator.h>
#include <MCP23017.h>
#include <SDCard.h>

extern I2CBus i2cBus;
extern Indicator indicator;
extern IOExt2 ioExt;
extern ADC adc;
extern DAC dac;
extern CarState carState;
extern CarControl carControl;
extern DriverDisplay driverDisplay;
extern EngineerDisplay engineerDisplay;
extern SDCard sdCard;

using namespace std;

unsigned long millisNextStamp = millis();

// ------------------
// FreeRTOS functions

void CarControl::re_init() { init(); }

void CarControl::init() {
  cout << "[?] Setup '" << getName() << "'...\n";
  justInited = true;
  mutex = xSemaphoreCreateMutex();
  xSemaphoreGive(mutex);
  adjust_paddles(5); // manually adjust paddles (5s handling time)
  sleep_polling_ms = 250;
  string s = fmt::format("[v] {} inited.\n", getName());
  cout << s;
  driverDisplay.print(s.c_str());
}

void CarControl::exit(void) {
  // TODO
}

void CarControl::_handleValueChanged() {
  // TODO
}
// ------------------

bool CarControl::read_battery_data() {
  carState.BatteryVoltage = adc.read(ADC::Pin::BAT_VOLTAGE) / 100.;  // TODO
  carState.BatteryCurrent = adc.read(ADC::Pin::BAT_CURRENT) / 1000.; // TODO
  return true;
}

bool CarControl::read_motor_data() {
  carState.MotorVoltage = adc.read(ADC::Pin::MOTOR_VOLTAGE) / 100.;  // TODO
  carState.MotorCurrent = adc.read(ADC::Pin::MOTOR_CURRENT) / 1000.; // TODO
  return true;
}

bool CarControl::read_pv_data() {
  carState.PhotoVoltaicCurrent = adc.read(ADC::Pin::PV_CURRENT) / 100.; // TODO
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
  carState.Speed = (int)speed;
  debug_printf_l3("raw %5d | %5.2f, rpm:%5.2f, speed:%5.2f, %4d\n", value, voltage, rpm, speed, (int)speed);
  return true;
}

bool CarControl::read_paddles() {
  bool hasChanged = false;
  if (carState.BreakPedal) {
    cout << "Break Pedal Pressed (paddle control)" << endl;
    _set_dec_acc_values(DAC_MAX, 0, ADC_MAX, 0, -88);
    return true;
  }
  int16_t valueDec = adc.read(ADC::Pin::STW_DEC);
  int16_t valueAcc = adc.read(ADC::Pin::STW_ACC);

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
  debug_printf("Dec (v0):  %5d  | Acc (v1): %5d  | ACCEL-DISPLAY: %3d ==> set POT0 =%3d (dec(%5d-%5d)), POT1 =%3d (acc(%5d-%5d))\n",
               valueDec, valueAcc, valueDisplay, valueDecPot, ads_min_dec, ads_max_dec, valueAccPot, ads_min_acc, ads_max_acc);

  dac.set_pot(valueDecPot, DAC::pot_chan::POT_CHAN1);
  dac.set_pot(valueAccPot, DAC::pot_chan::POT_CHAN0);

  carState.Deceleration = valueDec;
  carState.Acceleration = valueAcc;
  carState.AccelerationDisplay = valueDisplay;

  valueDisplayLast = valueDisplay;
}

void CarControl::adjust_paddles(int seconds) {
  int x, y;
  int16_t value;

  ads_min_acc = 50000;
  ads_min_dec = 50000;
  ads_max_acc = 0;
  ads_max_dec = 0;

  cout << DISPLAY_STATUS_str[(int)engineerDisplay.get_DisplayStatus()] << " | "
       << DISPLAY_STATUS_str[(int)driverDisplay.get_DisplayStatus()] << endl;

  int cycles = (seconds * 10);
  if (cycles < 1)
    cycles = 1;
  string s("    adjust...");
  cout << s;
  if (engineerDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
    carState.DriverInfo = s;
  } else {
    engineerDisplay.print(s.c_str());
    engineerDisplay.getCursor(x, y);
  }
  while (cycles-- > 0) {
    s = fmt::format(" {}", cycles);
    cout << s;
    if (engineerDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
      carState.DriverInfo = s;
    } else {
      engineerDisplay.write_ganz_99(x, y, cycles + 1, cycles, 1, false, ILI9341_BLUE, ILI9341_ORANGE);
    }

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
    delay(100);
  }
  // make sure null level to avoid automatic acceleration/deceleration
  ads_min_dec += 5000;
  ads_min_acc += 5000;

  s = fmt::format("\n    ==>dec {:5}-{:5} == acc {:5}-{:5}\n", ads_min_dec, ads_max_dec, ads_min_acc, ads_max_acc);
  cout << s;
  if (engineerDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
    carState.DriverInfo = fmt::format("==> dec {:5}-{:5}       ==> acc {:5}-{:5}", ads_min_dec, ads_max_dec, ads_min_acc, ads_max_acc);
  } else {
    engineerDisplay.print(s.c_str());
  }
}

int CarControl::_normalize(int minDisplayValue, int maxDisplayValue, int minValue, int maxValue, int value) {
  float displArea = maxDisplayValue - minDisplayValue;
  float valueArea = maxValue - minValue;

  float retValue = (value - minValue) * displArea / valueArea;

  return (int)retValue;
}

void CarControl::_handle_indicator() {}
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

    // one data row per second
    if (sdCard.logEnabled && (millis() > millisNextStamp)) {
      millisNextStamp = millis() + LOG_INTERVALL;
      sdCard.write(carState.csv("Recent State"));
    }

    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
