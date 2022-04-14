#ifndef __CONFIG_FILE_H__
#define __CONFIG_FILE_H__

// https://renenyffenegger.ch/notes/development/libraries/cpp/read-configuration-files/
// https://github.com/ReneNyffenegger/cpp-read-configuration-files

#include <map>
#include <string>

#include "Chameleon.h"

using namespace std;

class ConfigFile {
  std::map<string, Chameleon> content_;

public:
  ConfigFile(string const &configFile);

  Chameleon const &Value(string const &section, string const &entry) const;
  //Chameleon const &Value(string const &section, string const &entry, string const &default_value) const;
  string Value(string const &section, string const &entry, string const &default_value) const;

  Chameleon const &Value(string const &section, string const &entry, int value);
  Chameleon const &Value(string const &section, string const &entry, int value, int default_value);

  Chameleon const &Value(string const &section, string const &entry, float value);
  Chameleon const &Value(string const &section, string const &entry, float value, float default_value);

  Chameleon const &Value(string const &section, string const &entry, double value);
  Chameleon const &Value(string const &section, string const &entry, double value, double default_value);
};

#endif
