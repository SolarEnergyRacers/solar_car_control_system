//
// Car State with all car information
//

#include <definitions.h>

#include <CarState.h>

#include <DriverDisplayC.h>
#include <IOExt.h>
#include <Indicator.h>

using namespace std;

extern CONSTANT_MODE constant_drive_mode;
extern bool constant_drive_set;

string CarState::printableState() {
  stringstream ss;
  ss << "Car state\nSpeed=" << Speed << "\nAcceleration=" << Acceleration;
  return ss.str();
}

string CarState::serializedState() { return "...."; }
