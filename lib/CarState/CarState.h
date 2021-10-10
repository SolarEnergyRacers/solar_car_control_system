//
// Car State with all car information
//

#ifndef CARSTATE_H
#define CARSTATE_H

#include <cJSON.h>
#include <sstream>
#include <string>
#include <map>


#include <CarStatePin.h>
#include <CarStateValue.h>

#include <definitions.h>

using namespace std;

// public structures
enum class INDICATOR { OFF, LEFT, RIGHT, WARN };
enum class INFO_TYPE { INFO, STATUS, WARN, ERROR };
enum class CONSTANT_MODE { NONE, SPEED, POWER };
enum class DRIVE_DIRECTION { FORWARD, BACKWARD };

class CarState {

public:
  CarState() {
    Speed.set(321);
    Acceleration.set(-12);
    Deceleration.set(0);
    Battery.set(-13.5);
    Battery.set_epsilon(0.1);
    PhotoVoltaic.set(-1);
    PhotoVoltaic.set_epsilon(0.1);
    Motor.set(-1);
    Motor.set_epsilon(0.1);

    Indicator.set(INDICATOR::OFF);
    DriveDirection.set(DRIVE_DIRECTION::FORWARD);
    ConstantMode.set(CONSTANT_MODE::SPEED);

    TargetSpeed.set(0);
    TargetPower.set(0);
    InfoLast.set("ok.");
    Light1On.set(false);
    Light2On.set(false);
  }
  ~CarState(){};

  // pyhsical car data (measurment values)
  CarStateValue<int> Speed;
  CarStateValue<int> Acceleration;
  CarStateValue<int> Deceleration;

  CarStateValue<float> Battery;
  CarStateValue<float> PhotoVoltaic;
  CarStateValue<float> Motor;

  // logical car data (values set by driver or chase car)
  CarStateValue<DRIVE_DIRECTION> DriveDirection;
  CarStateValue<CONSTANT_MODE> ConstantMode;
  CarStateValue<INDICATOR> Indicator;

  CarStateValue<int> TargetSpeed;
  CarStateValue<int> TargetPower;
  CarStateValue<string> InfoLast;
  CarStateValue<bool> Light1On;
  CarStateValue<bool> Light2On;

  // All IO pins
  static CarStatePin pins[IOExtPINCOUNT];
  int getIdx(string pinName);
  CarStatePin *getPin(int devNr, int pinNr);
  CarStatePin *getPin(int port);
  CarStatePin *getPin(string pinName);

  std::map<string, int> idxOfPin;
  // std::map<int, Pin> pins; // pins by index
  
  // tools
  const string print(string msg);
  const string printIOs(string msg);
  const string serialize(string msg);
};

#endif // CARSTATE_H
