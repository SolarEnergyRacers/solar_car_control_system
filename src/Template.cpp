//
// This is an example class
//

//#include <stdlib.h>
#include <bits/stdc++.h>
#include <iomanip>
#include <iostream>
#include <string> // for string class
using namespace std;

#include "Template.h"

void Template::init(){
    // do initialization here
};

void Template::reset(){
    // handle reset here
};

void Template::exit(){
    // handle exit here
};

uint32_t Template::getValue() { return rand(); };

#define __CLASS__ std::remove_reference<decltype(classMacroImpl(this))>::type

template <class T> T &classMacroImpl(const T *t);

const char *Template::getInfo() {
  //   auto file = __FILE__;
  //   auto className = __PRETTY_FUNCTION__;
  //   string sep = "==";
  //   string val = file + sep + className;

  // from: src/Template.cpp==std::__cxx11::string Template::getInfo()
  // to:   src/Template.cpp: Template
  // string val2 = regex_replace(val.c_str(), regex("std::.* (.*)::.*"), "$1");
  //   return typeid(Template).name();
  //   return this->GetClass()->GetName();
  //string info = string();
  //info.append("Template (").append(__FILE__).append(")");
  //const char *retValue = info.c_str();
  //return retValue;
  return __FILE__;
};


