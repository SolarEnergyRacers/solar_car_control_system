//
// Car State with all car information
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CARSTATE_H
#define SOLAR_CAR_CONTROL_SYSTEM_CARSTATE_H

#include <CarStateValue.h>
#include <definitions.h>
#include <sstream>
#include <string>

using namespace std;

class CarState {

public:
  CarState() {
    Speed.set(-10);
    Speed.set_epsilon(0);
    Acceleration.set(0);
    Acceleration.set_epsilon(0);
    Deceleration.set(0);
    Deceleration.set_epsilon(0);
    Battery.set(-1);
    Battery.set_epsilon(0.1);
    PhotoVoltaic.set(-1);
    PhotoVoltaic.set_epsilon(0.1);
    Motor.set(-1);
    Motor.set_epsilon(0.1);
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
  CarStateValue<int> TargetSpeed;
  CarStateValue<int> TargetPower;
  CarStateValue<char *> infoLast;
  CarStateValue<bool> light1On;
  CarStateValue<bool> light2On;
  CarStateValue<bool> indicatorLeft;
  CarStateValue<bool> indicatorRigth;

  // tools
  // char* printableState();
  // char* serializedState();

  const char *printableState() {
    string s = "Car state:";
    stringstream ss(s);
    ss << endl << "Speed=" << Speed.get() << "\nAcceleration=" << Acceleration.get() << endl;
    return ss.str().c_str();
  }

  const char *serializedState() {
    float a = 3;
    float b = 5;
    float c = abs(b - a);
    return "....";
  }
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CARSTATE_H
