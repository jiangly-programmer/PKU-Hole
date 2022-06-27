#ifndef TREEHOLE_VERSION

#define TREEHOLE_VERSION "NO_CACHEFILE_TERMINAL_VERSION_1_0_0"

#include "basic_class.h"
#include "filter.h"
#include "headers.h"
#include "hole.h"
#include "hole_api.h"
#include "holecollection.h"

string trim(string s);

struct HoleSearcher {
  // 如果时间较短（或者 enforce_all ），那么可以分批次返回所有满足要求的树洞。
  // 否则，只有在正文中至少出现一个关键词的洞才会被找到。
  Filter filter;
  TimeFilter TFilter;
  bool enforce_all;
  int R;  // 如果必须抓取所有树洞，那么在一开始就把编号起始位置二分出来
  explicit HoleSearcher(const Filter&, const TimeFilter&, bool);
  const HoleCollection getNext();
};

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
