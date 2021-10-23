//
// Car State Values
//

#ifndef SER_CARSTATEVALUE_H
#define SER_CARSTATEVALUE_H

#include <stdio.h>  /* printf */
#include <stdlib.h> /* abs */

#include <definitions.h>
//#include <../lib/CarControl/CarControl.h>

using namespace std;

template <typename T> class CarStateValue {

private:
  T _epsilon;
  T _Value;
  T _ValueLast;

public:
  // set the new value and returns the last set value
  T set(T theValue) {
    _Value = theValue;
    // CarControl::valueChangedHandler();
    return _ValueLast;
  }
  // get the recent value
  T get() { return _Value; }
  // get the last value
  T get_last() { return _ValueLast; }
  // makes the recent value to the valueLast
  void overtake_recent_to_last() { _ValueLast = _Value; }
  void set_epsilon(T theEpsilon) { _epsilon = theEpsilon; }
  // check if there is a vaue difference bettween current and last
  bool is_changed() { return abs(_Value - _ValueLast) > _epsilon; }
};

#endif // SER_CARSTATEVALUE_H
