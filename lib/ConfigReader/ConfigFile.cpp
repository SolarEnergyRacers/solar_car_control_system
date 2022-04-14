#include "ConfigFile.h"

#include <fmt/core.h>
#include <fmt/printf.h>
#include <fstream>

#include <Console.h>
#include <SDCard.h>

extern Console console;
extern SDCard sdCard;

using namespace std;

string trim(string const &source, char const *delims = " \t\r\n") {
  string result(source);
  string::size_type index = result.find_last_not_of(delims);
  if (index != string::npos)
    result.erase(++index);

  index = result.find_first_not_of(delims);
  if (index != string::npos)
    result.erase(0, index);
  else
    result.erase();
  return result;
}

// read the whole config file and store the Section/Entry pairs in a map
ConfigFile::ConfigFile(string const &configFile) {
  sdCard.mount();
  if (sdCard.isMounted()) {
    try {

      ifstream file(configFile.c_str());
      console << fmt::format("START 3a READING FROM {}___________________\n", configFile);

      string line;
      string name;
      string value;
      string inSection;
      int posEqual;
      while (getline(file, line)) {
        console << fmt::format("START 3b READING FROM CONFIG.INI, line:{}___________________\n", line);

        if (!line.length())
          continue;

        if (line[0] == '#')
          continue;
        if (line[0] == ';')
          continue;

        if (line[0] == '[') {
          inSection = trim(line.substr(1, line.find(']') - 1));
          continue;
        }

        posEqual = line.find('=');
        name = trim(line.substr(0, posEqual));
        value = trim(line.substr(posEqual + 1));

        console << "START 3c READING FROM CONFIG.INI___________________\n";
        // remove line end comment
        posEqual = value.find('#');
        value = trim(value.substr(0, posEqual));
        content_[inSection + '/' + name] = Chameleon(value);
        console << "START 3d READING FROM CONFIG.INI___________________\n";
      }
    } catch (exception &ex) {
      console << "WARN: No readable configfile: '" << configFile << "' found: " << ex.what() << "\n";
    }
  } else {
    content_["no_config_info/not_found"] = Chameleon("no value");
    console << "WARN: No readable configfile: '" << configFile << "' found. Using coded settings.\n";
  }
}

Chameleon const &ConfigFile::Value(string const &section, string const &entry) const {

  std::map<string, Chameleon>::const_iterator ci = content_.find(section + '/' + entry);
  console << "START 4a READING FROM CONFIG.INI___________________\n";

  if (ci == content_.end())
    throw "does not exist";

  return ci->second;
}

// Chameleon const &ConfigFile::Value(string const &section, string const &entry, string const &default_value) const {

//   std::map<string, Chameleon>::const_iterator ci = content_.find(section + '/' + entry);
//   console << "START 4b READING FROM CONFIG.INI___________________\n";

//   if (ci == content_.end()) {
//     console << fmt::format("START 4c READING FROM CONFIG.INI, value={}___________________\n", default_value);
//     Chameleon const v = (Chameleon const)default_value;
//     console << fmt::format("START 4d READING FROM CONFIG.INI, value={}___________________\n", v);
//     return (Chameleon const)default_value;
//   }
//   console << "START 4e READING FROM CONFIG.INI___________________\n";
//   return ci->second;
// }

string ConfigFile::Value(string const &section, string const &entry, string const &default_value) const {

  std::map<string, Chameleon>::const_iterator ci = content_.find(section + '/' + entry);
  console << "START 4b READING FROM CONFIG.INI___________________\n";

  if (ci == content_.end()) {
    console << fmt::format("START 4c READING FROM CONFIG.INI, value={}___________________\n", default_value);
    string v = default_value;
    console << fmt::format("START 4d READING FROM CONFIG.INI, value={}___________________\n", v);
    return default_value;
  }
  console << "START 4e READING FROM CONFIG.INI___________________\n";
  return (string)ci->second;
}


Chameleon const &ConfigFile::Value(string const &section, string const &entry, double default_value) {
  try {
    return Value(section, entry);
  } catch (const char *) {
    return content_.insert(make_pair(section + '/' + entry, Chameleon(default_value))).first->second;
  }
}
