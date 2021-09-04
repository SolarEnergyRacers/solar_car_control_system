//
// Car State with all car information
//

#ifndef CARSTATE_H
#define CARSTATE_H

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

#endif // CARSTATE_H
