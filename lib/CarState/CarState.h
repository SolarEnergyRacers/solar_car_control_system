//
// Car State with all car information
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CARSTATE_H
#define SOLAR_CAR_CONTROL_SYSTEM_CARSTATE_H

#include <CarStateValue.h>
#include <cJSON.h>
#include <definitions.h>
#include <sstream>
#include <string>

// public structures
enum class INDICATOR { OFF, LEFT, RIGHT, WARN };
static const char *INDICATOR_str[] = {"off", "left", "right", "HAZARD FLASHR"};

enum class INFO_TYPE { INFO, STATUS, WARN, ERROR };
static const char *INFO_TYPE_str[] = {"Info", "Status", "Warning", "Error"};

enum class CONSTANT_MODE { NONE, SPEED, POWER };
static const char *CONSTANT_MODE_str[] = {"none", "speed", "power"};

enum class DRIVE_DIRECTION { FORWARD, BACKWARD };
static const char *DRIVE_DIRECTION_str[] = {"fwd", "bwd"};

static const char *BOOL_str[] = {"false", "true"};
static const char *ON_OFF_str[] = {"off", "on"};

using namespace std;

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
    // IndicatorLeft.set(false);
    // IndicatorRight.set(false);
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
  // CarStateValue<bool> IndicatorLeft;
  // CarStateValue<bool> IndicatorRight;

  //Pin *pins;

  // tools
  const string print(string msg);
  const string serialize(string msg);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CARSTATE_H
