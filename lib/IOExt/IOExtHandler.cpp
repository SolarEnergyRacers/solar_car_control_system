//
// MCP23017 I/O Extension over I2C
//
#include <definitions.h>

#include <stdio.h>

// standard libraries
#include <fmt/core.h>
#include <inttypes.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include <MCP23017.h> // MCP23017
#include <Wire.h>     // I2C

#include <CarControl.h>
#include <Console.h>
#include <DriverDisplay.h>
#include <EngineerDisplay.h>
#include <Helper.h>
#include <I2CBus.h>
#include <IOExt.h>
#include <IOExtHandler.h>
#include <SDCard.h>

extern Console console;
extern I2CBus i2cBus;
extern Indicator indicator;
extern IOExt ioExt;
extern CarState carState;
extern CarControl carControl;
extern SDCard sdCard;
extern bool systemOk;

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
  } else { // VCC_5V
    carState.BreakPedal = carState.getPin(PinBreakPedal)->value == 1;
  }
  if (!carState.BreakPedal) { // break pedal released
    carControl.reset_acceleration_values();
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
    if (carState.Light == LIGHT::L1) {
      carState.Light = LIGHT::OFF;
    } else {
      carState.Light = LIGHT::L1;
    }
    light_switch();
  }
}
void headLightHandler() {
  int value = carState.getPin(PinHeadLight)->value;
  if (value == 0) {
    if (carState.Light == LIGHT::L2) {
      carState.Light = LIGHT::L1;
    } else {
      carState.Light = LIGHT::L2;
    }
  }
  light_switch();
}
void light_switch() {
  console << "Light toggle\n";
  switch (carState.Light) {
  case LIGHT::OFF:
    carState.getPin(PinLightOut)->value = 0;
    carState.getPin(PinHeadLightOut)->value = 0;
    break;
  case LIGHT::L1:
    carState.getPin(PinLightOut)->value = 1;
    carState.getPin(PinHeadLightOut)->value = 0;
    break;
  case LIGHT::L2:
    carState.getPin(PinLightOut)->value = 1;
    carState.getPin(PinHeadLightOut)->value = 1;
    break;
  }
}

void nextScreenHandler() {
  if (!systemOk)
    return;
  if (carState.getPin(PinNextScreen)->value != 0)
    return;

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

void constantModeOnHandler() {
  if (!systemOk)
    return;
  if (carState.getPin(PinConstantModeOn)->value != 0)
    return;

  console << "ConstantMode ON";
  if (carState.ConstantModeOn) {
    carState.TargetSpeed = carState.Speed;                                       // unit: km/h
    carState.TargetPower = carState.MotorCurrent * carState.MotorVoltage / 1000; // unit: kW
    console << " - overtake speed " << carState.TargetSpeed << " / power " << carState.TargetPower;
  }
  console << "\n";
  carState.ConstantModeOn = true; // #SAFETY#: deceleration unlock const mode
}

void constantModeOffHandler() {
  if (!systemOk)
    return;
  if (carState.getPin(PinConstantModeOff)->value != 0)
    return;

  if (carState.ConstantModeOn) {
    console << "ConstantMode OFF\n";
    carState.ConstantModeOn = false; // #SAFETY#: deceleration unlock const mode
  }
}

void constantModeHandler() {
  if (!systemOk)
    return;
  if (carState.getPin(PinConstantMode)->value != 0)
    return;

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

void paddleAdjustHandler() {
  if (!systemOk)
    return;
  if (carState.getPin(PinPaddleAdjust)->value != 0)
    return;

  if (carState.ControlMode == CONTROL_MODE::PADDLES) {
    if (carState.getPin(PinPaddleAdjust)->value == 0) {
      console << "Request Paddle Adjust\n";
      carControl.adjust_paddles(carState.PaddleAdjustCounter); // manually adjust paddles (3s handling time)
    }
  } else {
    carState.ButtonControlModeIncrease = carState.ButtonControlModeIncrease == carState.ButtonControlModeIncreaseLow
                                             ? carState.ButtonControlModeIncreaseHeigh
                                             : carState.ButtonControlModeIncreaseLow;
    console << fmt::format("Set ButtonControlModeIncrease to {}\n", carState.ButtonControlModeIncrease);
  }
}

void controlModeHandler() {
  if (!systemOk)
    return;
  if (carState.getPin(PinControlMode)->value != 0)
    return;
  if (carState.ControlMode == CONTROL_MODE::PADDLES) {
    console << fmt::format("Control mode {}\n", CONTROL_MODE_str[int(CONTROL_MODE::BUTTONS)]);
    carState.ControlMode = CONTROL_MODE::BUTTONS;
    carControl.reset_acceleration_values();
  } else {
    carState.ControlMode = CONTROL_MODE::PADDLES;
    console << fmt::format("Control mode {}\n", CONTROL_MODE_str[int(CONTROL_MODE::PADDLES)]);
  }
}

void sdCardDetectHandler() {
  carState.SdCardDetect = carState.getPin(PinSdCardDetect)->value == 1;
  if (carState.SdCardDetect) {
    console << "SD card detected, try to start logging...\n";
    string msg = sdCard.init();
    console << msg << "\n";
    string state = carState.csv("Recent State", true); // with header
    sdCard.write(state);
    sdCard.open_log_file();
  } else {
    console << "SD card removed.\n";
  }
}

void decreaseHandler() {
  if (!systemOk || !carState.ConstantModeOn) // || carState.ControlMode == CONTROL_MODE::BUTTONS)
    return;
  carState.TargetSpeed -= carState.ConstSpeedIncrease;
  if (carState.TargetSpeed < 0)
    carState.TargetSpeed = 0;

  carState.TargetPower -= carState.ConstPowerIncrease;
  if (carState.TargetPower < 0)
    carState.TargetPower = 0;

  console << "Decrease constant mode target to " << carState.TargetSpeed << "km/h / " << carState.TargetPower << "W.\n";
}

void increaseHandler() {
  if (!systemOk || !carState.ConstantModeOn) // || carState.ControlMode == CONTROL_MODE::BUTTONS)
    return;
  carState.TargetSpeed += carState.ConstSpeedIncrease;
  if (carState.TargetSpeed > 111) // only until 111km/h
    carState.TargetSpeed = 111;

  carState.TargetPower += carState.ConstPowerIncrease;
  if (carState.TargetPower > 4500) // only until 5kW
    carState.TargetPower = 4500;

  console << "Increase constant mode target to " << carState.TargetSpeed << "km/h / " << carState.TargetPower << " W.\n";
}
