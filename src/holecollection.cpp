#include "holecollection.h"

HoleCollection::HoleCollection() {}
HoleCollection::HoleCollection(const std::vector<int>& hid) {
  auto res = API.multi_getone(hid);
  // for (auto h:hid) std::cerr<<h<<' '; std::cerr<<'\n';
  for (const auto& x : res) {
    // std::cerr << JSON_to_string(x) << '\n';
    if (x["code"].asInt() == 0) {
      holes.emplace_back(x, HoleJSON::GETONE);
      if (holes.back().pid == 0) {
        std::cerr << JSON_to_string(x) << std::endl;
        assert(0);
      }
    }
  }
}
HoleCollection::HoleCollection(const HoleCollection& hid) : holes(hid.holes) {}

const HoleCollection HoleCollection::from_search_result(const JSON& x) {
  std::cerr << "enter HoleCollection::from_search_result()" << std::endl;
  // std::cerr << JSON_to_string(x) << std::endl;
  HoleCollection ret;
  for (int i = 0; i < (int)x["data"].size(); i++) {
    ret.holes.emplace_back(x["data"][i], HoleJSON::GETLIST);
  }
  std::cerr << "exit HoleCollection::from_search_result()" << std::endl;
  return ret;
}
const HoleCollection HoleCollection::from_getlist_result(const JSON& x) {
  HoleCollection ret;
  for (int i = 0; i < (int)x["data"].size(); i++) {
    ret.holes.emplace_back(x["data"][i], HoleJSON::GETLIST);
  }
  return ret;
}
void HoleCollection::sort_by_pid() {
  std::sort(holes.begin(), holes.end(),
            [](const Hole& a, const Hole& b) { return a.pid < b.pid; });
}

string HoleCollection::to_str() const {
  std::string x;
  x += "-------------------------------------\n";
  for (const auto& w : holes) {
    x += w.to_str();
    x += "-------------------------------------\n";
  }
  return x;
}

void HoleCollection::updateAll() {
  vector<int> pid;
  for (const auto& w : holes) {
    pid.push_back(w.pid);
  }
  auto res = API.multi_getcomment(pid);
  for (int i = 0; i < (int)res.size(); i++) {
    if (res[i]["code"].asInt() == 0) {
      for (int j = 0; j < (int)res[i]["data"].size(); j++) {
        holes[i].reply[j] = Text(res[i]["data"][j]["text"].asString());
      }
    }
  }
}
const HoleCollection HoleCollection::merge(const HoleCollection& rhs) const {
  HoleCollection ret = *this;
  ret.holes.insert(ret.end(), rhs.begin(), rhs.end());
  return ret;
}

HoleCollection::iterator HoleCollection::begin() {
  return holes.begin();
}
HoleCollection::iterator HoleCollection::end() {
  return holes.end();
}
HoleCollection::const_iterator HoleCollection::begin() const {
  return holes.cbegin();
}
HoleCollection::const_iterator HoleCollection::end() const {
  return holes.cend();
}