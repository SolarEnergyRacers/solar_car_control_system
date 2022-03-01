
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

// class teebuf: public std::streambuf
// {
// public:
//     // Construct a streambuf which tees output to both input
//     // streambufs.
//     teebuf(std::streambuf * sb1, std::streambuf * sb2)
//         : sb1(sb1)
//         , sb2(sb2)
//     {
//     }
// private:
//     // This tee buffer has no buffer. So every character "overflows"
//     // and can be put directly into the teed buffers.
//     virtual int overflow(int c)
//     {
//         if (c == EOF)
//         {
//             return !EOF;
//         }
//         else
//         {
//             int const r1 = sb1->sputc(c);
//             int const r2 = sb2->sputc(c);
//             return r1 == EOF || r2 == EOF ? EOF : c;
//         }
//     }

//     // Sync both teed buffers.
//     virtual int sync()
//     {
//         int const r1 = sb1->pubsync();
//         int const r2 = sb2->pubsync();
//         return r1 == 0 && r2 == 0 ? 0 : -1;
//     }
// private:
//     std::streambuf * sb1;
//     std::streambuf * sb2;
// };

// class Console : public std::ostream {
class Console {
public:
  string buffer = "";

  // https://wordaligned.org/articles/cpp-streambufs
  //   Console(std::ostream &o1, std::ostream &o2);

  // private:
  //   teebuf tbuf;

  // Console(std::ostream & dst, std::ostream & src)
  //       : src(src), sbuf(src.rdbuf(dst.rdbuf())) {}
  // ~Console() { src.rdbuf(sbuf); }
  // ostream &src;
  // streambuf *const sbuf;

public:
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
