
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

using namespace std;

class Console {
public:
  string buffer;

public:
  // //------- OUT ---------
  // // friend Console &operator<<(Console &c, const object &obj) {
  // //   c.buffer = to_string(obj);
  // //   cout << c.buffer;
  // //   Serial2 << c.buffer.c_str();
  // //   return c;
  // // }
  // friend Console &operator<<(Console &c, const string &str) {
  //   c.buffer = str;
  //   cout << c.buffer;
  //   if (Serial2)
  //     Serial2 << c.buffer.c_str();
  //   return c;
  // }

  // friend Console &operator<<(Console &c, const char &chr) {
  //   c.buffer = to_string(chr);
  //   cout << c.buffer;
  //   Serial2 << c.buffer.c_str();
  //   return c;
  // }

  // friend Console &operator<<(Console &c, const int &i) {
  //   c.buffer = to_string(i);
  //   cout << c.buffer;
  //   Serial2 << c.buffer.c_str();
  //   return c;
  // }

  // friend Console &operator<<(Console &c, const float &f) {
  //   c.buffer = to_string(f);
  //   cout << c.buffer;
  //   Serial2 << c.buffer.c_str();
  //   return c;
  // }

  // friend Console &operator<<(Console &c, const bool &b) {
  //   c.buffer = to_string(b);
  //   cout << c.buffer;
  //   Serial2 << c.buffer.c_str();
  //   return c;
  // }

  // //-------- IN ---------
  // friend string &operator>>(string &s, Console &c) {
  //   c.buffer = s;
  //   cout << c.buffer;
  //   Serial2 << c.buffer.c_str();
  //   return c.buffer;
  // }

  //------- OUT ---------
  friend Console &operator<<(Console &c, const string &str);
  friend Console &operator<<(Console &c, const char &chr);
  friend Console &operator<<(Console &c, const int &i);
  friend Console &operator<<(Console &c, const float &f);
  friend Console &operator<<(Console &c, const bool &b);

  //-------- IN ---------
  friend string &operator>>(string &s, Console &c);
};

#endif // SOLAR_CAR_CONTROL_SYSTEM_CONSOLE_H
