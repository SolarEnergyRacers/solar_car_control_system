//
// Car State Values
//

#ifndef SER_CARSTATEVALUE_H
#define SER_CARSTATEVALUE_H

#include <definitions.h>

#include <iostream>
#include <stdio.h>
#include <string>

#include <CarState.h>

using namespace std;

template <typename T> class CarStateValue {

private:
  T _epsilon;
  T _Value;
  T _ValueLast;

public:
  // CarStateValue(T value, T epsilon) {
  //   _Value = value;
  //   _epsilon = epsilon;
  //   _ValueLast = _Value - 2 * _epsilon;
  // }
  // ~CarStateValue() {}

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
  T get_recent_overtake_last() {
    _ValueLast = _Value;
    return _Value;
  }
  void epsilon(T theEpsilon) { _epsilon = theEpsilon; }
  T epsilon() { return _epsilon; }
  // check if there is a vaue difference bettween current and last
  bool is_changed() { return _Value != _ValueLast; };
};

#endif // SER_CARSTATEVALUE_H
