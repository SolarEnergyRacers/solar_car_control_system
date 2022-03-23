#include <definitions.h>

#include <Streaming.h>

#include <iostream>
#include <string>

#include <Console.h>

using namespace std;

extern Console console;

bool invalidChar(char c) {
  return !(c >= 19 && c < 128 && c != 9 && c != 10 && c != 13);
  // return !isprint( static_cast<unsigned char>( c ) );
}
void stripUnicode(string &str) { str.erase(remove_if(str.begin(), str.end(), invalidChar), str.end()); }

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
  cout.flush();
  // string s = string(str);
  // stripUnicode(s);
  // Serial2 << s.c_str();
  Serial2 << str;
  Serial2.flush();
  return c;
}

//-------- IN ---------
string &operator>>(string &s, Console &c) {
  cout << s;
  Serial2 << s.c_str();
  return c.buffer;
}
