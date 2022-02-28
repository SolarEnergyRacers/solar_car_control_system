
#include <Streaming.h>
#include <iostream>
#include <string>

#include <Console.h>

using namespace std;

extern Console console;

//------- OUT ---------
// friend Console &operator<<(Console &c, const object &obj) {
//   c.buffer = to_string(obj);
//   cout << c.buffer;
//   Serial2 << c.buffer.c_str();
//   return c;
// }
Console &operator<<(Console &c, const string &str) {
  c.buffer = str;
  cout << c.buffer;
  if (Serial2)
    Serial2 << c.buffer.c_str();
  return c;
}

Console &operator<<(Console &c, const char &chr) {
  c.buffer = to_string(chr);
  cout << c.buffer;
  Serial2 << c.buffer.c_str();
  return c;
}

Console &operator<<(Console &c, const int &i) {
  c.buffer = to_string(i);
  cout << c.buffer;
  Serial2 << c.buffer.c_str();
  return c;
}

Console &operator<<(Console &c, const float &f) {
  c.buffer = to_string(f);
  cout << c.buffer;
  Serial2 << c.buffer.c_str();
  return c;
}

Console &operator<<(Console &c, const bool &b) {
  c.buffer = to_string(b);
  cout << c.buffer;
  Serial2 << c.buffer.c_str();
  return c;
}

//-------- IN ---------
string &operator>>(string &s, Console &c) {
  c.buffer = s;
  cout << c.buffer;
  Serial2 << c.buffer.c_str();
  return c.buffer;
}
