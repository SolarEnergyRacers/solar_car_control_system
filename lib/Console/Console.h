
//
// Software Serial Console Streaming object
//
// https://bingmann.github.io/2007/0301-StdOStreamPrintable.html
// https://stackoverflow.com/questions/22714089/no-match-for-operator-operand-types-stdostream-c-oop-and-point
// std::cout << myPoint is just syntactic sugar for operator<<(std::cout, myPoint).

#ifndef SOLAR_CAR_CONTROL_SYSTEM_CONSOLE_H
#define SOLAR_CAR_CONTROL_SYSTEM_CONSOLE_H

// #include <Streaming.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include <fstream>
#include <iostream>

using namespace std;

class Console {
public:
  string buffer = "";

  //------- OUT ---------
  friend Console &operator<<(Console &c, const bool &var) { return operator<<(c, to_string(var)); }
  friend Console &operator<<(Console &c, const char &var) { return operator<<(c, to_string(var)); }
  friend Console &operator<<(Console &c, const double &var) { return operator<<(c, to_string(var)); }
  friend Console &operator<<(Console &c, const float &var) { return operator<<(c, to_string(var)); }
  friend Console &operator<<(Console &c, const int &var) { return operator<<(c, to_string(var)); }
  friend Console &operator<<(Console &c, const long &var) { return operator<<(c, to_string(var)); }
  friend Console &operator<<(Console &c, const size_t &var) { return operator<<(c, to_string(var)); }
  friend Console &operator<<(Console &c, const string &var) { return operator<<(c, var.c_str()); }
  friend Console &operator<<(Console &c, const unsigned long &var) { return operator<<(c, to_string(var)); }
  friend Console &operator<<(Console &c, const uint64_t &var) { return operator<<(c, to_string(var)); }
  friend Console &operator<<(Console &c, const volatile int &var) { return operator<<(c, to_string(var)); }

  friend Console &operator<<(Console &c, const char *str);

  //-------- IN ---------
  friend string &operator>>(string &s, Console &c);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CONSOLE_H
