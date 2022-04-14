#ifndef __CONFIG_FILE_H__
#define __CONFIG_FILE_H__

// https://renenyffenegger.ch/notes/development/libraries/cpp/read-configuration-files/
// https://github.com/ReneNyffenegger/cpp-read-configuration-files

#include <map>
#include <string>

using namespace std;

class ConfigFile {
  std::map<string, string> content_;

public:
  ConfigFile(string const &configFile);

  string const &get(string const &section, string const &entry, string default_value) const;
  bool get(string const &section, string const &entry, bool default_value);
  int get(string const &section, string const &entry, int default_value);
  float get(string const &section, string const &entry, float default_value);
  double get(string const &section, string const &entry, double default_value);
};

#endif
