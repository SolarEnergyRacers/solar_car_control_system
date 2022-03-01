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
Console &operator<<(Console &c, const string &str) {
  cout << str;
  Serial2 << str.c_str();
  cout.flush();
  Serial2.flush();
  return c;
}

Console &operator<<(Console &c, const char &chr) {
  cout << to_string(chr);
  Serial2 << to_string(chr).c_str();
  cout.flush();
  Serial2.flush();
  return c;
}

Console &operator<<(Console &c, const int &i) {
  cout << to_string(i);
  Serial2 << to_string(i).c_str();
  cout.flush();
  Serial2.flush();
  return c;
}

Console &operator<<(Console &c, const float &f) {
  cout << to_string(f);
  Serial2 << to_string(f).c_str();
  return c;
}

Console &operator<<(Console &c, const bool &b) {
  // c.buffer = to_string(b);
  cout << to_string(b);
  Serial2 << to_string(b).c_str();
  return c;
}

//-------- IN ---------
string &operator>>(string &s, Console &c) {
  cout << s;
  Serial2 << s.c_str();
  return c.buffer;
}
