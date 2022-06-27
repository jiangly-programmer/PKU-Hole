#include "config.h"

std::string CONFIG::url = config["url"].asString() + "api.php";
std::string CONFIG::token = config["token"].asString();