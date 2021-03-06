#include "HoleSearcher.h"

// TODO: 2000000?

HoleCollection HoleSearcherCache::update(int once) {
  // std::cerr << "enter HoleSearcherCache::update()" << std::endl;
  int last;
  if (cache.size())
    last = cache.back().first;
  else
    last = 2000000;
  HoleCollection now_ = HoleCollection::from_getlist_result(API.getlist(1));
  if (last + 2015 > now_.holes.front().pid || time(0) - update_time < 3600) {
    return now_;
  }
  update_time = time(0);
  int first_time = 1;
  while (last + 2015 <= now_.holes.front().pid) {
    std::cerr << "updating cache, please wait...\n";
    if (!first_time) {
      Sleep(30000);
    }
    first_time = 0;
    vector<int> V;
    for (int i = 1; i <= 15; i++) {
      last += 6000 + rand() % 10;
      if (last > now_.holes.front().pid)
        break;
      V.push_back(last);
    }
    HoleCollection res(V);
    for (auto h : res)
      if (h.hole_time < cache.back().second) {
        std::cerr << "ERROR" << std::endl;
        std::cerr << "h.hole_time < cache.back().second" << std::endl;
        std::cerr << "cache.back().second: " << cache.back().second
                  << std::endl;
        std::cerr << "h.hole_time: " << h.hole_time << std::endl;
        std::cerr << h.to_str() << std::endl;
        for (auto t : V)
          std::cerr << t << ' ';
        std::cerr << '\n';
        std::cerr << h.pid;
        exit(0);
      }
    std::cerr << last << "finished, " << res.holes.size() << " holes\n";
    FILE* F = fopen("cache/HoleSearcherCache", "a");
    for (auto h : res)
      cache.push_back({h.pid, h.hole_time}),
          fprintf(F, "%d %llu\n", h.pid, h.hole_time);
    fclose(F);
    if (res.holes.size() < 10 && last <= now_.holes.front().pid) {
      std::cerr << "WARNING: res.holes.size() < 10\n";
      Sleep(30000);
    }
    if (once) break;
  }
  return now_;
}
HoleSearcherCache::HoleSearcherCache() {
  FILE* F = fopen("cache/HoleSearcherCache", "r");
  if (F != NULL) {
    int x;
    time_t y;
    while (fscanf(F, "%d %llu", &x, &y) == 2)
      cache.push_back({x, y});
    fclose(F);
  }
}
HoleSearcherCache HSC;

HoleSearcher::HoleSearcher(const Filter& _filter,
                           const TimeFilter& tfilter,
                           bool enforce)
    : filter(_filter), TFilter(tfilter), enforce_all(enforce) {
  // std::cerr << "enter HoleSearcher\n";
  L = 1e9;
  if (enforce_all || !filter.allPositiveWords().size()) {
    HoleCollection now_ = HSC.update();
    for (auto h : now_)
      if (h.hole_time <= TFilter.time_max) {
        R = h.pid;
        return;
      }
    int l, r;
    if (HSC.cache.front().second > TFilter.time_max) {
      R = 0;
      return;
    }
    if (HSC.cache.back().second <= TFilter.time_max)
      r = now_.holes.back().pid, l = HSC.cache.back().second;
    else {
      for (int i = (int)HSC.cache.size() - 2; i >= 0; i--)
        if (HSC.cache[i].second <= TFilter.time_max) {
          r = HSC.cache[i + 1].first, l = HSC.cache[i].first;
          break;
        }
    }
    while (r - l > 20) {
      vector<int> V;
      for (int i = 1; i <= 20; i++)
        V.push_back(l + i * (r - l) / 20);
      HoleCollection cur(V);
      assert(cur.holes.size());
      if (cur.holes.back().hole_time <= TFilter.time_max) {
        l = cur.holes.back().pid;
        continue;
      }
      for (int i = (int)cur.holes.size() - 2; i >= 0; i--)
        if (cur.holes[i].hole_time <= TFilter.time_max) {
          r = cur.holes[i + 1].pid, l = cur.holes[i].pid;
          break;
        }
    }
    R = r;
  } else
    R = 1;
  // std::cerr << "exit HoleSearcher\n";
}

const HoleCollection HoleSearcher::getNext(unsigned cnt) {
  printf("enter HoleSearcher::getNext\n");
  auto words = filter.allPositiveWords();
  printf("positive words get\n");
  if (enforce_all || !words.size()) {
    HoleCollection res;
    while (R > 2000000 + 20 && res.holes.size() < cnt) {
      vector<int> V;
      for (int i = R; i > R - 20; i--)
        V.push_back(i);
      R -= 20;
      HoleCollection tmp(V);
      // tmp.updateAll();
      for (auto h : tmp)
        if (filter(h) && TFilter(h))
          res.holes.push_back(h);
      if (tmp.holes.back().hole_time < TFilter.time_min)
        break;
    }
    return res;
  }
  int first_time = 1;
  while (R <= 3) {
    int c = 0;
    for (auto h : buffer)
      if (h.pid >= L)
        ++c;
    if (c >= cnt / 2 && (!first_time || c >= cnt))
      break;
    first_time = 0;
    // buffer ????????????????????????????????????
    L = 0;
    set<int> s;
    for (auto h : buffer)
      s.insert(h.pid);
    for (auto word : words) {
      std::cerr << "try word " << word << ", page " << R << '\n';
      HoleCollection hc =
          HoleCollection::from_search_result(API.search(word, R));
      int last = 0;
      // hc.updateAll();
      for (auto h : hc)
        if (!s.count(h.pid)) {
          s.insert(h.pid);
          if (filter(h) && TFilter(h))
            buffer.insert(h), last = h.pid;
        }
      L = std::max(L, last);
      Sleep(200);
    }
    R++;
  }
  HoleCollection res;
  while (buffer.size() && std::prev(buffer.end())->pid >= L &&
         res.holes.size() < cnt) {
    res.holes.push_back(*std::prev(buffer.end()));
    buffer.erase(std::prev(buffer.end()));
  }
  return res;
}

void test_hole_searcher() {
  int year = 2022;
  int month = 2;
  int day = 10;
  int hour = 18;
  int minute = 30;
  int second = 0;
  std::tm _tm({second, minute, hour, day, month, year});
  time_t t = std::mktime(&_tm);
  TimeFilter TFilter(0, t);
  Filter Filter = ALLOK;
  HoleSearcher HS(Filter, TFilter, 1);
  HoleCollection res = HS.getNext();
  std::ofstream("tmp.out") << res.to_str() << '\n';
}