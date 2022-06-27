#include "filter.h"

TimeFilter::TimeFilter(size_t l, size_t r) : time_min(l), time_max(r) {}
bool TimeFilter::operator()(const Hole& h) const {
  return time_min <= h.hole_time && h.hole_time <= time_max;
}
TimeFilter T_ALLOK(0, ULLONG_MAX);

Filter::Filter(KeywordFilterType _type, const std::vector<Filter>& _son)
    : type(_type), son(_son), search_range(0) {
  assert(_type != KeywordFilterType::Leaf);
}
Filter::Filter(KeywordFilterType _type,
               const std::string& _word,
               int _search_range)
    : type(_type), word(_word), search_range(_search_range) {
  assert(_type == KeywordFilterType::Leaf);
}
bool Filter::operator()(const Hole& h) const {
  // std::cerr << "enter Filter::operator ()\n";
  if (type == KeywordFilterType::Leaf) {
    if (word == "") {
      // std::cerr << "exit KeywordFilter::operator ()\n";
      return true;
    }
    int res = h.main.haveWord(word);
    for (const auto& r : h.reply)
      if (r.haveWord(word)) {
        res |= 2;
        break;
      }
    // std::cerr << "exit KeywordFilter::operator ()\n";
    return res & search_range;
  } else {
    if (type == KeywordFilterType::Not) {
      assert(son.size() == 1);
      return !son[0](h);
    }
    if (type == KeywordFilterType::And) {
      for (const auto& f : son)
        if (!f(h))
          return false;
      return true;
    }
    if (type == KeywordFilterType::Or) {
      for (const auto& f : son)
        if (f(h))
          return true;
      return false;
    }
  }
  assert(false);
  return 0;
}
const set<string> Filter::allPositiveWords(bool cur) const {
  if (type == KeywordFilterType::Leaf) {
    return (cur && word != "") ? set<string>{word} : set<string>{};
  } else {
    if (type == KeywordFilterType::Not)
      return son[0].allPositiveWords(!cur);
    set<string> ret;
    for (const auto& f : son) {
      auto res = f.allPositiveWords(cur);
      ret.insert(res.begin(), res.end());
    }
    return ret;
  }
}
Filter ALLOK(Filter::KeywordFilterType::Leaf, "", 0x03);

Filter getFilter(const std::vector<std::pair<string, int>>& keywords) {
  // keywords[0] or keywords[1] or ...
  std::vector<Filter> tmp;
  for (const auto& w : keywords) {
    tmp.push_back(KeywordFilter(w.first, w.second));
  }
  return Filter(Filter::KeywordFilterType::Or, tmp);
}
Filter getFilter(
    const std::vector<std::vector<std::pair<string, int>>>& keywords) {
  // (keywords[0][0] or keywords[0][1] or ...) and (...)
  std::vector<Filter> res;
  for (const auto& w : keywords) {
    std::vector<Filter> tmp;
    for (const auto& p : w) {
      tmp.push_back(KeywordFilter(p.first, p.second));
    }
    res.emplace_back(Filter(Filter::KeywordFilterType::Or, tmp));
  }
  return Filter(Filter::KeywordFilterType::And, res);
}
Filter getFilter(const std::vector<std::pair<string, int>>& keywords,
                 const std::vector<std::pair<string, int>>& unwanted) {
  Filter A = getFilter(keywords);
  Filter B = Filter(Filter::KeywordFilterType::Not,
                    std::vector<Filter>{getFilter(unwanted)});
  return Filter(Filter::KeywordFilterType::Or, std::vector<Filter>{A, B});
}
Filter getFilter(
    const std::vector<std::vector<std::pair<string, int>>>& keywords,
    const std::vector<std::pair<string, int>>& unwanted) {
  // (keywords[0][0] or keywords[0][1] or ...) and (...)
  Filter A = getFilter(keywords);
  Filter B = Filter(Filter::KeywordFilterType::Not,
                    std::vector<Filter>{getFilter(unwanted)});
  return Filter(Filter::KeywordFilterType::And, std::vector<Filter>{A, B});
}