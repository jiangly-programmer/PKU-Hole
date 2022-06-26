#ifndef TREEHOLE_CONFIG
#define TREEHOLE_CONFIG
#include "basic_class.h"
class CONFIG {
	public:
	static std::string url;
	static std::string token;
};

namespace {
static JSON config = readJSON("config.json");
}
std::string CONFIG::url = config["url"].asString() + "api.php";
std::string CONFIG::token = config["token"].asString();

#endif