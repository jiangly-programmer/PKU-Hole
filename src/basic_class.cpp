#include "basic_class.h"

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
  std::cerr << "readJson\n";
  FILE* file = fopen(s.c_str(), "r");
  assert(file);
  static const int size = 1 << 20;
  static char buffer[size];
  int L = fread(buffer, 1, size, file);
  fclose(file);
  assert(L < size);
  return string_to_JSON(std::string(buffer, buffer + L));
}