#include <definitions.h>

#include <Streaming.h>

#include <iostream>
#include <string>

#include <Console.h>

using namespace std;

extern Console console;

//------- OUT ---------
// TODO: Is it possible to this with base object?
// friend Console &operator<<(Console &c, const object &obj) {
//   c.buffer = to_string(obj);
//   cout << c.buffer;
//   Serial2 << c.buffer.c_str();
//   return c;
// }

// Console &operator<<(Console &c, const string &str) {
//   // cout << "[string] ";
//   cout << str;
//   Serial2 << str.c_str();
//   cout.flush();
//   Serial2.flush();
//   return c;
// }

// Console &operator<<(Console &c, const char *str) {
//   // cout << "[char*] ";
//   cout << str;
//   Serial2 << str;
//   cout.flush();
//   Serial2.flush();
//   return c;
// }

// Console &operator<<(Console &c, const char &chr) {
//   // cout << "[char] ";
//   cout << chr;
//   Serial2 << to_string(chr).c_str();
//   cout.flush();
//   Serial2.flush();
//   return c;
// }

// Console &operator<<(Console &c, const int &i) {
//   // cout << "[int] ";
//   cout << i;
//   Serial2 << to_string(i).c_str();
//   cout.flush();
//   Serial2.flush();
//   return c;
// }

// Console &operator<<(Console &c, const volatile int &i) {
//   // cout << "[v int] ";
//   cout << i;
//   Serial2 << to_string(i).c_str();
//   cout.flush();
//   Serial2.flush();
//   return c;
// }

// Console &operator<<(Console &c, const long &l) {
//   // cout << "[long] ";
//   cout << l;
//   Serial2 << to_string(l).c_str();
//   cout.flush();
//   Serial2.flush();
//   return c;
// }

// Console &operator<<(Console &c, const unsigned long &l) {
//   // cout << "[u long] ";
//   cout << l;
//   Serial2 << to_string(l).c_str();
//   cout.flush();
//   Serial2.flush();
//   return c;
// }

// Console &operator<<(Console &c, const float &f) {
//   // cout << "[float] ";
//   cout << f;
//   Serial2 << to_string(f).c_str();
//   return c;
// }

// Console &operator<<(Console &c, const double &d) {
//   // cout << "[double] ";
//   cout << d;
//   Serial2 << to_string(d).c_str();
//   return c;
// }

// Console &operator<<(Console &c, const size_t &st) {
//   // cout << "[double] ";
//   cout << st;
//   Serial2 << to_string(st).c_str();
//   return c;
// }

// Console &operator<<(Console &c, const bool &b) {
//   // c.buffer = to_string(b);
//   // cout << "[bool] ";
//   cout << to_string(b);
//   Serial2 << to_string(b).c_str();
//   return c;
// }

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
