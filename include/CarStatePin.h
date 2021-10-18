//
// Car State Pins
//

#ifndef CARSTATEPIN_H
#define CARSTATEPIN_H

#include <stdio.h>  /* printf */
#include <stdlib.h> /* abs */
#include <string>

#include <definitions.h>

using namespace std;

class CarStatePin {
public:
  int port; // high nibble: device number, low nibble: portNr (pin)
  int mode;
  int value;
  int oldValue;
  long timestamp;
  string name;
  void (*handlerFunction)();
};

#endif // CARSTATEPIN_H
