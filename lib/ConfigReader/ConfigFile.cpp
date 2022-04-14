#include "ConfigFile.h"

#include <fmt/core.h>
#include <fmt/printf.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

        // remove line end comment
        posEqual = value.find('#');
        value = trim(value.substr(0, posEqual));
        content_[inSection + '/' + name] = value;
      }
      console << fmt::format("INFO: 3d:: READING FROM CONFIG.INI___________________\n", configFile);
    } catch (exception &ex) {
      console << "WARN: No readable configfile: '" << configFile << "' found: " << ex.what() << "\n";
    }
  } else {
    content_["no_config_info/not_found"] = "no value";
    console << "WARN: No readable configfile: '" << configFile << "' found. Using coded settings.\n";
  }
}

string const &ConfigFile::Value(string const &section, string const &entry, string default_value) const {

  std::map<string, string>::const_iterator ci = content_.find(section + '/' + entry);

  if (ci == content_.end())
    return default_value;

  return ci->second;
}

bool ConfigFile::Value(string const &section, string const &entry, bool default_value) {

  std::map<string, string>::const_iterator ci = content_.find(section + '/' + entry);

  if (ci == content_.end()) {
    return default_value;
  }
  bool value = default_value;
  istringstream(ci->second) >> value;
  return value;
}

int ConfigFile::Value(string const &section, string const &entry, int default_value) {

  std::map<string, string>::const_iterator ci = content_.find(section + '/' + entry);

  if (ci == content_.end()) {
    return default_value;
  }
  return stoi(ci->second);
}

float ConfigFile::Value(string const &section, string const &entry, float default_value) {

  std::map<string, string>::const_iterator ci = content_.find(section + '/' + entry);

  if (ci == content_.end()) {
    return default_value;
  }
  return stof(ci->second);
}

double ConfigFile::Value(string const &section, string const &entry, double default_value) {

  std::map<string, string>::const_iterator ci = content_.find(section + '/' + entry);

  if (ci == content_.end()) {
    return default_value;
  }
  return stod(ci->second);
}
