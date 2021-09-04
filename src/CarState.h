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

  // pyhsical car data

public:
  // set the new mesured speed and returns the last set speed
  CarStateValue Speed;
  CarStateValue Acceleration;
  CarStateValue Deceleration;
  
  // logical car data
  CarStateValue TargetSpeed;
  CarStateValue TargetPower;

  // tools
  string printableState();
  string serializedState();
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CARSTATE_H
