#ifndef TREEHOLE_HOLE_API
#define TREEHOLE_HOLE_API

#include "utility.h"
#include "config.h"

struct HoleAPI {
  JSON getjson(const std::string& url) const;
  JSON getone(int pid) const;
  vector<JSON> multi_getone(const vector<int>& pid) const;
  JSON getcomment(int pid) const;
  vector<JSON> multi_getcomment(const vector<int>& pid) const;
  JSON getlist(int p) const;
  JSON getattention() const;
  JSON search(string word, int page) const;
};
extern HoleAPI API;

#endif