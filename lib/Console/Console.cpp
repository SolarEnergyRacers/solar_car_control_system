
#include <Streaming.h>
#include <iostream>
#include <string>

#include <Console.h>

using namespace std;

extern Console console;

ostream &operator<<(ostream &os, const string &obj) {
  os << obj;
  Serial2 << obj;
  return os;
}

istream &operator>>(istream &is, string &obj) {
  is >> obj;
  // obj >> Serial2;
  return is;
}
