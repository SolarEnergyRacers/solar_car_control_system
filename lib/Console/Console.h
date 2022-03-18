
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

public:
  //------- OUT ---------
  friend Console &operator<<(Console &c, const string &str);
  friend Console &operator<<(Console &c, const char &chr);
  friend Console &operator<<(Console &c, const char *str);
  friend Console &operator<<(Console &c, const int &i);
  friend Console &operator<<(Console &c, const volatile int &i);
  friend Console &operator<<(Console &c, const long &l);
  friend Console &operator<<(Console &c, const unsigned long &l);
  friend Console &operator<<(Console &c, const float &f);
  friend Console &operator<<(Console &c, const double &d);
  friend Console &operator<<(Console &c, const size_t &st);
  friend Console &operator<<(Console &c, const bool &b);

  //-------- IN ---------
  friend string &operator>>(string &s, Console &c);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CONSOLE_H
