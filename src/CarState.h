//
// Car State with all car information
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CARSTATE_H
#define SOLAR_CAR_CONTROL_SYSTEM_CARSTATE_H

#include <string>
#include <definitions.h>
#include <CarStateValue.h>

using namespace std;

class CarState {

public:
  // CarState();
  // virtual ~CarState();
  
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
  CarStateValue<string> infoLast;
  CarStateValue<bool> light1On;
  CarStateValue<bool> light2On;
  CarStateValue<bool> indicatorLeft;
  CarStateValue<bool> indicatorRigth;

  // tools
  string printableState();
  string serializedState();

};

// void TemporaryFunction2 ()
// {
//     CarStateValue<int> tmpInt;
//     CarStateValue<float> tmpFloat;
//     CarStateValue<bool> tmpBool;
//     CarState tempCs;
// }

#endif // SOLAR_CAR_CONTROL_SYSTEM_CARSTATE_H
