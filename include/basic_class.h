#ifndef TREEHOLE_BASIC_CLASS
#define TREEHOLE_BASIC_CLASS

#include "headers.h"
#include "json/json.h"

typedef Json::Value JSON;

struct Image {
  string url;
  enum ImageType {
    Image_Cat,
    Image_Dog,
    Image_Person,
    Image_Scenery,
    Image_Text,
    Image_Else
  } type;
  explicit Image(const Image&);
  explicit Image(const string& x = "which sb do not fill url");
};

struct Text {
  string s;
  set<string> words;
  Text(const Text& x);
  explicit Text(const string& _s = "");
  bool haveWord(const string&) const;
};

extern Json::Reader reader;
extern Json::FastWriter writer;

JSON string_to_JSON(const string&);
string JSON_to_string(const JSON&);
JSON readJSON(const string&);
#endif