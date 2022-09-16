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

unsigned long millisNextStampCsv = millis();
unsigned long millisNextStampSnd = millis();

// ------------------
// FreeRTOS functions

string CarControl::re_init() { return init(); }

string CarControl::init() {
  bool hasError = false;
  console << "[  ] Init 'CarControl'...\n";
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
  // Comes from CAN bus: carState.BatteryVoltage = adc.BAT_VOLTAGE / 100.;  // TODO
  // Comes from CAN bus: carState.BatteryCurrent = adc.BAT_CURRENT / 1000.; // TODO
#endif
  return true;
}

bool CarControl::read_motor_data() {
#if ADC_ON
  carState.MotorVoltage = adc.MOTOR_VOLTAGE / 100.;  // TODO
  carState.MotorCurrent = adc.MOTOR_CURRENT / 1000.; // TODO
#endif
  return true;
}

bool CarControl::read_pv_data() {
#if ADC_ON
  // Comes from CAN bus: carState.PhotoVoltaicCurrent = adc.PV_CURRENT / 100.; // TODO
#endif
  return true;
}

bool CarControl::read_reference_cell_data() {
#if ADC_ON
  carState.ReferenceSolarCell = adc.REFERENCE_CELL; // TODO
#endif
  return true;
}

bool CarControl::read_speed() {
  float diameter = 0.50; // m
#if ADC_ON
  int16_t value = adc.MOTOR_SPEED; // native input
  float voltage = value * adc.get_multiplier(ADC::Pin::MOTOR_SPEED_PORT);
  float rpm = 370 * voltage;                                   // round per minute
  carState.Speed = round(3.1415 * diameter * rpm * 6. / 100.); // unit: km/h
#endif
  return true;
}

int CarControl::calculate_displayvalue_acc_dec(int valueDec, int valueAcc) {
  int16_t valueDecNorm = 0;
  int16_t valueAccNorm = 0;
  int valueDisplay = 0;
  valueDecNorm = CarControl::_normalize(0, MAX_ACCELERATION_DISPLAY_VALUE, ads_min_dec, ads_max_dec, valueDec);
  if (valueDecNorm > 0) {
    valueDisplay = -valueDecNorm;
  } else {
    valueAccNorm = CarControl::_normalize(0, MAX_ACCELERATION_DISPLAY_VALUE, ads_min_acc, ads_max_acc, valueAcc);
    if (valueAccNorm > 0) {
      valueDisplay = valueAccNorm;
    } else {
      valueDisplay = 0;
    }
  }
  return valueDisplay;
}

bool CarControl::read_PLUS_MINUS() {
  bool hasChanged = false;
  if (carState.BreakPedal) {
    _set_dec_acc_values(DAC_MAX, 0, ADC_MAX, 0, -64);
    return true;
  }

  bool plusButton = ioExt.getPort(carState.getPin(PinIncrease)->port) == 0;  // currently pressed?
  bool minusButton = ioExt.getPort(carState.getPin(PinDecrease)->port) == 0; // currently pressed?
  if (!plusButton && !minusButton)
    return false;

  int8_t acceleration; // -99 ... +99
  if (carState.Deceleration > 0) {
    acceleration = -carState.Deceleration;
  } else {
    acceleration = carState.Acceleration;
  }

  if (minusButton) {
    if (acceleration - carState.ButtonControlModeIncrease > -99)
      acceleration -= carState.ButtonControlModeIncrease;
    else
      acceleration = -99;
  } else if (plusButton) {
    if (acceleration + carState.ButtonControlModeIncrease < 99)
      acceleration += carState.ButtonControlModeIncrease;
    else
      acceleration = 99;
  }

  int8_t valueDec = 0;
  int8_t valueAcc = 0;
  if (acceleration < 0) {
    valueDec = -acceleration;
  } else if (acceleration > 0) {
    valueAcc = acceleration;
  }

  int valueDisplay = valueDec > 0 ? -valueDec : valueAcc;
  // prepare and write motor acceleration and recuperation values to DigiPot
  int valueDecPot = 0;
  int valueAccPot = 0;
  if (valueDisplay < 0) {
    valueDecPot = -(int)(((float)valueDisplay / MAX_ACCELERATION_DISPLAY_VALUE) * DAC_MAX);
    valueAccPot = 0;
  } else {
    valueDecPot = 0;
    valueAccPot = (int)(((float)valueDisplay / MAX_ACCELERATION_DISPLAY_VALUE) * DAC_MAX);
  }

  if (valueDisplayLast != valueDisplay) {
    hasChanged = true;
    _set_dec_acc_values(valueDecPot, valueAccPot, valueDec, valueAcc, valueDisplay);
  }
  if (verboseMode) {
    console << fmt::format(
        "button mode: acc={:3d} --> valueDecPot={:4d}, valueAccPot={:4d} | valueDec={:3d}, valueAcc={:3d}, valueDisplay={:3d}\n",
        acceleration, valueDecPot, valueAccPot, valueDec, valueAcc, valueDisplay);
  }
  return hasChanged;
}

bool CarControl::read_paddles() {
  bool hasChanged = false;
  if (carState.BreakPedal) {
    _set_dec_acc_values(DAC_MAX, 0, ADC_MAX, 0, -64);
    return true;
  }
  int16_t valueDec = adc.STW_DEC;
  int16_t valueAcc = adc.STW_ACC;
  // check if change is in damping
  // if (valueAcc != 0 && valueDec != 0)
  //   if (abs(accelLast - valueAcc) < carState.PaddleDamping && abs(recupLast - valueDec) < carState.PaddleDamping)
  //     return false;

  // #SAFETY#: Reset constant mode on deceleration paddel touched
  if (valueDec > carState.PaddleDamping) {
    carState.ConstantModeOn = false;
  }

  int valueDisplay = calculate_displayvalue_acc_dec(valueDec, valueAcc);
  // prepare and write motor acceleration and recuperation values to DigiPot
  int valueDecPot = 0;
  int valueAccPot = 0;
  if (valueDisplay < 0) {
    valueDecPot = -(int)(((float)valueDisplay / MAX_ACCELERATION_DISPLAY_VALUE) * DAC_MAX);
    valueAccPot = 0;
  } else {
    valueDecPot = 0;
    valueAccPot = (int)(((float)valueDisplay / MAX_ACCELERATION_DISPLAY_VALUE) * DAC_MAX);
  }

  if (valueDisplayLast != valueDisplay) {
    hasChanged = true;
  }
  _set_dec_acc_values(valueDecPot, valueAccPot, valueDec, valueAcc, valueDisplay);
  if (verboseMode) {
    console << fmt::format("paddle mode: valueDecPot={:4d}, valueAccPot={:4d} | valueDec={:3d}, valueAcc={:3d}, valueDisplay={:3d}\n",
                           valueDecPot, valueAccPot, valueDec, valueAcc, valueDisplay);
  }
  return hasChanged;
}

void CarControl::_set_dec_acc_values(int valueDecPot, int valueAccPot, int16_t valueDec, int16_t valueAcc, int valueDisplay) {
  if (carState.ConstantModeOn && valueDec == 0 && valueAcc == 0)
    return;
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
  string s("    adjust: ");
  if (engineerDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
    carState.DriverInfo = s;
  } else {
    engineerDisplay.setCursor(4, 70);
    engineerDisplay.print(s.c_str());
    engineerDisplay.getCursor(x, y);
  }
  while (cycles-- > 0) {
    s = fmt::format("paddle adjust: {:2d}\n", cycles);
    console << s;
    if (engineerDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
      carState.DriverInfo = s;
    } else {
      engineerDisplay.write_ganz_99(x + 2, y, cycles + 1, cycles, 1, true, ILI9341_BLUE, ILI9341_WHITE);
    }

#if ADC_ON
    int16_t value;
    value = adc.read(ADC::Pin::STW_DEC_PORT);
    if (value > 0) {
      if (ads_min_dec > value)
        ads_min_dec = value;
      if (ads_max_dec < value)
        ads_max_dec = value;
    }
    value = adc.read(ADC::Pin::STW_ACC_PORT);
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
  delay(1000);
  string result = carState.AccelerationDisplay == 0 ? "ok" : (carState.AccelerationDisplay == -64) ? "BRK?" : "ERR";
  s = fmt::format("=> dec {:5}-{:5}        => acc {:5}-{:5} => {}", ads_min_dec, ads_max_dec, ads_min_acc, ads_max_acc, result);
  console << "\n    " << s << "\n";
  if (engineerDisplay.get_DisplayStatus() == DISPLAY_STATUS::DRIVER_RUNNING) {
    carState.DriverInfo = s;
  }
  if (result == "ok") {
    delay(640);
    carState.DriverInfo = "";
  }
  //#SAFTY#
  carState.PaddlesAdjusted = true;
}

int CarControl::_normalize(int minDisplayValue, int maxDisplayValue, int minValue, int maxValue, int value) {
  float displArea = maxDisplayValue - minDisplayValue;
  float valueArea = maxValue - minValue;
  if (value < minValue)
    value = minValue;
  if (value > maxValue)
    value = maxValue;
  float retValue = (value - minValue) * displArea / valueArea;

  return round(retValue);
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
    // read values from ADC/IO
    carState.getPin(PinGreenLightOut)->value = carState.GreenLight ? 1 : 0;

    // update OUTPUT pins
    ioExt.writeAllPins(PinHandleMode::FORCED);

    carState.getPin(PinDacLifeSign)->value = !carState.getPin(PinDacLifeSign)->value;

    // handle changed INPUT pins
    bool someThingChanged = false;
    if (carState.ControlMode == CONTROL_MODE::PADDLES)
      someThingChanged |= read_paddles();
    else if (!carState.ConstantModeOn)
      someThingChanged |= read_PLUS_MINUS();
    someThingChanged |= read_motor_data();
    someThingChanged |= read_battery_data();
    someThingChanged |= read_pv_data();
    someThingChanged |= read_speed();
    someThingChanged |= read_reference_cell_data();

    _handle_indicator();

    // one data row per second
    if ((millis() > millisNextStampCsv) || (millis() > millisNextStampSnd)) {
      string record = carState.csv();
      if (sdCard.isReadyForLog() && millis() > millisNextStampCsv) {
        sdCard.write(record);
        millisNextStampCsv = millis() + carState.LogInterval;
      }
      if (millis() > millisNextStampSnd) {
        console << "d: " + record;
        millisNextStampSnd = millis() + carState.CarDataSendPeriod;
      }
    }
    // sleep
    vTaskDelay(sleep_polling_ms / portTICK_PERIOD_MS);
  }
}
