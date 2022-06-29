#ifndef TREEHOLE_VERSION

#define TREEHOLE_VERSION "NO_CACHEFILE_TERMINAL_VERSION_1_0_0"

#include "basic_class.h"
#include "Filter.h"
#include "headers.h"
#include "Hole.h"
#include "HoleAPI.h"
#include "HoleCollection.h"
#include "HoleSearcher.h"

string trim(string s);

struct TagManager {
  map<string, HoleCollection> tags;
  const HoleCollection getTag(const string& tag);
  void addHole(const Hole&, const string&);
  void updateTag(const string&);
  TagManager();
  ~TagManager();
};
extern TagManager tagManager;

struct TerminalInterater {
  void main();
};

extern TerminalInterater terminalInterater;

#endif
