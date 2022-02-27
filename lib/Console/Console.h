
//
// Software Serial Console Streaming object
//
// https://bingmann.github.io/2007/0301-StdOStreamPrintable.html
// https://stackoverflow.com/questions/22714089/no-match-for-operator-operand-types-stdostream-c-oop-and-point

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CONSOLE_H
#define SOLAR_CAR_CONTROL_SYSTEM_CONSOLE_H

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

class Console {
public:
  friend ostream &operator<<(ostream &, const string &);
  friend istream &operator>>(istream &, string &);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CONSOLE_H
