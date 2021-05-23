//
// This is an example class
//

#ifndef TEMPLATE_H
#define TEMPLATE_H

//#include <stdint.h>
#include <iostream>
#include <string> // for string class
using namespace std;

class Template {
private:
  // put private/internal variables/functions here
public:
  void init();
  void reset();
  void exit();
  uint32_t getValue();
  const char* getInfo();
};

#endif // TEMPLATE_H
