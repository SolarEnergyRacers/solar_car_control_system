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
    Speed = 0;
    Acceleration = 0;
    Deceleration = 0;
    BatteryVoltage = 0;
    BatteryCurrent = 0;
    PhotoVoltaicCurrent = 0;
    MotorCurrent = 0;

    Indicator = INDICATOR::OFF;
    DriveDirection = DRIVE_DIRECTION::FORWARD;
    ConstantMode = CONSTANT_MODE::SPEED;
    ConstantModeOn = false;

    TargetSpeed = 0;
    TargetPower = 0;
    DriverInfo = "ok.";
    DriverInfoType = INFO_TYPE::STATUS;
    Light = LIGHT::OFF;
  }
  ~CarState(){};

  // pyhsical car data (measurment values)
  int Speed;               // ADC
  int Acceleration;        // ADC Steering Wheel
  int Deceleration;        // ADC Steering Wheel
  int AccelerationDisplay; // Display Value (-99...+99)

  bool BatteryOn;            // IO-In
  float BatteryVoltage;      // CAN
  float BatteryCurrent;      // CAN
  bool PhotoVoltaicOn;       // IO-in
  float PhotoVoltaicCurrent; // CAN
  bool MotorOn;              // IO-In
  float MotorVoltage;        // ADC
  float MotorCurrent;        // ADC

  float Mppt1Current; // CAN
  float Mppt2Current; // CAN
  float Mppt3Current; // CAN

  float Umin; // CAN
  float Uavg; // CAN
  float Umax; // CAN

  bool BreakPedal;

  // logical car data (values set by driver or chase car)
  DRIVE_DIRECTION DriveDirection;
  CONSTANT_MODE ConstantMode;
  bool ConstantModeOn;
  INDICATOR Indicator;
  bool IndicatorBlink;

  int TargetSpeed;
  int TargetPower;
  string DriverInfo;
  SPEED_ARROW SpeedArrow;
  INFO_TYPE DriverInfoType;
  LIGHT Light;

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
