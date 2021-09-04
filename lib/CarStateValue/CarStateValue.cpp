//
// Car State with all car information
//

#include <definitions.h>
#include <CarStateValue.h>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* abs */


using namespace std;

// set the new mesured speed and returns the last set speed
int CarStateValue::set(int theValue) {
  _Value = theValue;
  return _ValueLast;
}
// get the recent speed
int CarStateValue::get() { return _Value; }
// get the last speed
int CarStateValue::get_last() { return _ValueLast; }
// makes the recent speed to the SpeedLast
void CarStateValue::overtake_recent_to_last() { _ValueLast = _Value; }
// check if there is a Speed difference bettween current and last
bool CarStateValue::is_changed() { return abs(_Value - _ValueLast) > _epsilon; }
