#include <definitions.h>

#include <Streaming.h>

#include <iostream>
#include <malloc.h>
#include <stdio.h>
#include <string>

#include <Console.h>

using namespace std;

bool static extended_charset(char c) { return c < ' ' && c != '\t' && c != '\n' && c != '\r'; }

const char *strip_extended_chars(string str) {
  str.erase(remove_if(str.begin(), str.end(), extended_charset), str.end());
  return str.c_str();
}

//------- OUT ---------
Console &operator<<(Console &c, const char *str) {
  if (!(str[0] == 'd' && str[1] == ':')) {
    cout << str;
    cout.flush();
  }
#if SERIAL_RADIO_ON
  Serial2 << strip_extended_chars(string(str));
  Serial2.flush();
#endif

  // // buffered transfer
  //  size_t packageSize = 20;
  //  char *buf = (char *)malloc(packageSize * sizeof(char) + 1); // package size + terminator
  //  int len = strlen(str);
  //  for (int i = 0; i * packageSize <= len; i++) {
  //    strncpy(buf, str + (i * packageSize), packageSize);
  //    *(buf + packageSize) = 0;
  //    Serial2 << strip_extended_chars(string(buf));
  //    Serial2 << buf;
  //  }
  //  free(buf);
  //  Serial2.flush();
  return c;
}

//-------- IN ---------
string &operator>>(string &s, Console &c) {
  cout << s;
  Serial2 << s.c_str();
  return c.buffer;
}
