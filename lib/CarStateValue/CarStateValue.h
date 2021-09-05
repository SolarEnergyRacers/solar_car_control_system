//
// Car State Values
//

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CARSTATEVALUE_H
#define SOLAR_CAR_CONTROL_SYSTEM_CARSTATEVALUE_H

#include <definitions.h>
#include <stdio.h>  /* printf */
#include <stdlib.h> /* abs */

using namespace std;

template <typename T> class CarStateValue {

private:
  T _epsilon = 0;
  T _Value = 0;
  T _ValueLast = 0;

public:
  // set the new value and returns the last set value
  T set(T theValue) {
    _Value = theValue;
    return _ValueLast;
  }
  // get the recent value
  T get() { return _Value; }
  // get the last value
  T get_last() { return _ValueLast; }
  // makes the recent value to the valueLast
  void overtake_recent_to_last() { _ValueLast = _Value; }
  // check if there is a vaue difference bettween current and last
  bool is_changed() { return abs(_Value - _ValueLast) > _epsilon; }
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CARSTATEVALUE_H
