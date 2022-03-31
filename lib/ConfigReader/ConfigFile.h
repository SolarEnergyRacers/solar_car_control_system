#ifndef __CONFIG_FILE_H__
#define __CONFIG_FILE_H__

// https://renenyffenegger.ch/notes/development/libraries/cpp/read-configuration-files/
// https://github.com/ReneNyffenegger/cpp-read-configuration-files

#include <map>
#include <string>

#include "Chameleon.h"

class ConfigFile {
  std::map<std::string, Chameleon> content_;

public:
  ConfigFile(std::string const &configFile);

  Chameleon const &Value(std::string const &section, std::string const &entry) const;

  Chameleon const &Value(std::string const &section, std::string const &entry, double value);
  Chameleon const &Value(std::string const &section, std::string const &entry, std::string const &value);
};

#endif
