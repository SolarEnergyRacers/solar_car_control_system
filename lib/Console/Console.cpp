#include <definitions.h>

#include <Streaming.h>

#include <iostream>
#include <string>

#include <Console.h>

using namespace std;

extern Console console;

//------- OUT ---------
Console &operator<<(Console &c, const bool &var) { return operator<<(c, to_string(var)); }
Console &operator<<(Console &c, const char &var) { return operator<<(c, to_string(var)); }
Console &operator<<(Console &c, const double &var) { return operator<<(c, to_string(var)); }
Console &operator<<(Console &c, const float &var) { return operator<<(c, to_string(var)); }
Console &operator<<(Console &c, const int &var) { return operator<<(c, to_string(var)); }
Console &operator<<(Console &c, const long &var) { return operator<<(c, to_string(var)); }
Console &operator<<(Console &c, const size_t &var) { return operator<<(c, to_string(var)); }
Console &operator<<(Console &c, const string &var) { return operator<<(c, var.c_str()); }
Console &operator<<(Console &c, const unsigned long &var) { return operator<<(c, to_string(var)); }
Console &operator<<(Console &c, const volatile int &var) { return operator<<(c, to_string(var)); }

Console &operator<<(Console &c, const char *str) {
  cout << str;
  Serial2 << str;
  cout.flush();
  Serial2.flush();
  return c;
}

//-------- IN ---------
string &operator>>(string &s, Console &c) {
  cout << s;
  Serial2 << s.c_str();
  return c.buffer;
}
