//
// Car State with all car information
//

#include <CarState.h>
#include <DriverDisplayC.h>
#include <definitions.h>
#include <string>

using namespace std;

// CarState::CarState() {
//   Speed.set(0);
//   Acceleration.set(0);
//   Deceleration.set(0);
//   Battery.set(-1);
//   PhotoVoltaic.set(-1);
//   Motor.set(-1);
// }

string CarState::printableState() {
  // stringstream ss;
  // ss << "Car state\nSpeed=" << Speed << "\nAcceleration=" << Acceleration;
  // return ss.str();
  return "------";
}

string CarState::serializedState() {
  float a = 3;
  float b = 5;
  float c = abs(b - a);
  return "....";
}
