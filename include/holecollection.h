#pragma once

#include "Filter.h"
#include "Hole.h"

struct HoleCollection {
  vector<Hole> holes;

  explicit HoleCollection();
  explicit HoleCollection(const std::vector<int>& hid);
  HoleCollection(const HoleCollection& hid);

  static const HoleCollection from_search_result(const JSON& x);
  static const HoleCollection from_getlist_result(const JSON& x);
  void sort_by_pid();

  string to_str() const;

  void updateAll();
  const HoleCollection filter(const Filter&) const;
  const HoleCollection merge(const HoleCollection& rhs) const;
  JSON to_Json() const;

  typedef vector<Hole>::iterator iterator;
  typedef vector<Hole>::const_iterator const_iterator;
  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
};
