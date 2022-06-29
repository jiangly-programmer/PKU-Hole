#pragma once

#include "Filter.h"
#include "HoleCollection.h"

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

struct HoleSearcherCache {
  vector<std::pair<int, time_t>> cache;  // 大约每一千条存几个位置
  time_t update_time;
  HoleCollection update();
  HoleSearcherCache();
};

extern HoleSearcherCache HSC;

void test_hole_searcher();