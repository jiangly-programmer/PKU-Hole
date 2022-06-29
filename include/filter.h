#pragma once

#include "Hole.h"

struct TimeFilter {
  size_t time_min, time_max;

  TimeFilter(size_t, size_t);
  virtual bool operator()(const Hole& h) const;
};
extern TimeFilter T_ALLOK;

struct Filter {
  enum KeywordFilterType { And, Or, Not, Leaf } type;
  std::vector<Filter> son;
  string word;
  int search_range;

  Filter(KeywordFilterType, const std::vector<Filter>&);
  Filter(KeywordFilterType, const std::string&, int);
  bool operator()(const Hole&) const;
  const set<string> allPositiveWords(bool cur = true) const;
};
extern Filter ALLOK;

static Filter KeywordFilter(const std::string& _word, int _search_range) {
  return Filter(Filter::KeywordFilterType::Leaf, _word, _search_range);
}
Filter getFilter(const std::vector<std::pair<string, int>>&);
Filter getFilter(const std::vector<std::vector<std::pair<string, int>>>&);
Filter getFilter(const std::vector<std::pair<string, int>>&,
                 const std::vector<std::pair<string, int>>&);
Filter getFilter(const std::vector<std::vector<std::pair<string, int>>>&,
                 const std::vector<std::pair<string, int>>&);
