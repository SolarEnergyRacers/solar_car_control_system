//
// Car State with all car information
//

#ifndef CARSTATE_H
#define CARSTATE_H

#include <cJSON.h>
#include <map>
#include <sstream>
#include <string>

#include <CarStatePin.h>
#include <CarStateValue.h>
#include <definitions.h>

using namespace std;

// public structures
enum class INDICATOR { OFF, LEFT, RIGHT, WARN };
enum class INFO_TYPE { INFO, STATUS, WARN, ERROR };
enum class SPEED_ARROW { OFF, INCREASE, DECREASE };
enum class CONSTANT_MODE { NONE, SPEED, POWER };
enum class DRIVE_DIRECTION { FORWARD, BACKWARD };
enum class LIGHT { OFF, L1, L2 };

class CarState {

public:
  CarState() {
    Speed.set(0);
    Acceleration.set(0);
    Deceleration.set(0);
    BatteryVoltage.set(0);
    BatteryVoltage.set_epsilon(0.1);
    BatteryCurrent.set(0);
    BatteryCurrent.set_epsilon(0.1);
    PhotoVoltaicCurrent.set(0);
    PhotoVoltaicCurrent.set_epsilon(0.1);
    MotorCurrent.set(0);
    MotorCurrent.set_epsilon(0.1);

    Indicator.set(INDICATOR::OFF);
    DriveDirection.set(DRIVE_DIRECTION::FORWARD);
    ConstantMode.set(CONSTANT_MODE::SPEED);
    ConstantModeOn.set(false);

    TargetSpeed.set(0);
    TargetPower.set(0);
    DriverInfo.set("ok.");
    DriverInfoType.set(INFO_TYPE::STATUS);
    Light.set(LIGHT::OFF);
  }
  ~CarState(){};

  // pyhsical car data (measurment values)
  CarStateValue<int> Speed;               // ADC
  CarStateValue<int> Acceleration;        // ADC Steering Wheel
  CarStateValue<int> Deceleration;        // ADC Steering Wheel
  CarStateValue<int> AccelerationDisplay; // Display Value (-99...+99)

  CarStateValue<bool> BatteryOn;            // IO-In
  CarStateValue<float> BatteryVoltage;      // CAN
  CarStateValue<float> BatteryCurrent;      // CAN
  CarStateValue<bool> PhotoVoltaicOn;       // IO-in
  CarStateValue<float> PhotoVoltaicCurrent; // CAN
  CarStateValue<bool> MotorOn;              // IO-In
  CarStateValue<float> MotorVoltage;        // ADC
  CarStateValue<float> MotorCurrent;        // ADC

  CarStateValue<float> Mppt1Current; // CAN
  CarStateValue<float> Mppt2Current; // CAN
  CarStateValue<float> Mppt3Current; // CAN

  CarStateValue<float> Umin; // CAN
  CarStateValue<float> Uavg; // CAN
  CarStateValue<float> Umax; // CAN

  CarStateValue<bool> BreakPedal;

  // logical car data (values set by driver or chase car)
  CarStateValue<DRIVE_DIRECTION> DriveDirection;
  CarStateValue<CONSTANT_MODE> ConstantMode;
  CarStateValue<bool> ConstantModeOn;
  CarStateValue<INDICATOR> Indicator;
  CarStateValue<bool> IndicatorBlink;

  CarStateValue<int> TargetSpeed;
  CarStateValue<int> TargetPower;
  CarStateValue<string> DriverInfo;
  CarStateValue<SPEED_ARROW> SpeedArrow;
  CarStateValue<INFO_TYPE> DriverInfoType;
  CarStateValue<LIGHT> Light;

  // All IO pins
  static CarStatePin pins[IOExtPINCOUNT];
  int getIdx(string pinName);
  CarStatePin *getPin(int devNr, int pinNr);
  CarStatePin *getPin(int port);
  CarStatePin *getPin(string pinName);

  std::map<string, int> idxOfPin;
  // std::map<int, Pin> pins; // pins by index

  // tools
  const string print(string msg, bool withColors = true);
  const string printIOs(string msg, bool withColors = true);
  const string serialize(string msg);
};

#endif // CARSTATE_H
