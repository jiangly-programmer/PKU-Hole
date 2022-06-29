#include "utility.h"

string trim(string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
  return s;
}

Image::Image(const Image& x) : url(x.url) {}
Image::Image(const string& x) : url(x) {}

Text::Text(const Text& x) : s(x.s), words(x.words) {}
Text::Text(const string& _s) : s(_s) {}
bool Text::haveWord(const string& _s) const {
  return s.find(_s) != string::npos;
}

Json::Reader reader;
Json::FastWriter writer;

JSON string_to_JSON(const string& s) {
  JSON ret;
  reader.parse(s, ret);
  return ret;
}
string JSON_to_string(const JSON& s) {
  return writer.write(s);
}
JSON readJSON(const string& s) {
  FILE* file = fopen(s.c_str(), "r");
  assert(file);
  static const int size = 1 << 20;
  static char buffer[size];
  int L = fread(buffer, 1, size, file);
  fclose(file);
  assert(L < size);
  return string_to_JSON(std::string(buffer, buffer + L));
}