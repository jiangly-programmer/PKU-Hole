#ifndef TREEHOLE_CONFIG
#define TREEHOLE_CONFIG
#include "basic_class.h"
class CONFIG {
 public:
  static std::string url;
  static std::string token;
};
//static JSON config = readJSON("config.json");
static JSON config;

#endif