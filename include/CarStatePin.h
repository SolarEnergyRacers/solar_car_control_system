//
// Car State Pins
//

#ifndef CARSTATEPIN_H
#define CARSTATEPIN_H

#include <definitions.h>

using namespace std;

class CarStatePin {
public:
  int port; // high nibble: device number, low nibble: portNr (pin)
  int mask;
  int mode;
  int value;
  int oldValue;
  bool inited;
  unsigned long timestamp;
  string name;
  void (*handlerFunction)();
};

#endif // CARSTATEPIN_H
