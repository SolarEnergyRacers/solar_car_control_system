//
// Car State with all car information
//

#ifndef CARSTATEVALUE_H
#define CARSTATEVALUE_H

#include <definitions.h>

using namespace std;

class CarStateValue {

  // pyhsical car data
private:
  int _epsilon = 0;
  int _Value = 0;
  int _ValueLast = 0;

public:
  // set the new mesured speed and returns the last set speed
  int set(int theSpeed);
  // get the recent speed
  int get();
  // get the last speed
  int get_last();
  // makes the recent speed to the SpeedLast
  void overtake_recent_to_last();
  // check if there is a Speed difference bettween current and last
  bool is_changed();

  // logical car data
};

#endif // CARSTATEVALUE_H
