#include "PKUHole.h"
#include "head.h"
#include "HoleCollection.h"

using std::cin;
using std::cout;

void test_holecollection() {
  HoleCollection x({1, 3629474, 3377010});
  std::cout << x.to_str() << '\n';
}
int main(int argc, char* argv[]) {
  //test_hole_searcher();
  QApplication app(argc, argv);

  PKUHole window;
  window.show();

  return app.exec();

  // terminalInterater.main();
  // test_holecollection();

  // test_hole_searcher();
  // test_hole_api();
  // HoleCollection x = HoleCollection::from_getlist_result(API.getlist(1));
  // std::ofstream("test.json") << API.getlist(1) << '\n';
  // std::ofstream("test.json") << API.search("tqc", 0) << '\n';
  // HoleCollection x = HoleCollection::from_search_result(API.search("tqc",
  // 0)); cout << x.to_str() << '\n'; Filter
  // AFilter(Filter::KeywordFilterType::And, new KeywordFilter("出", 1),
  //                new KeywordFilter("山地车", 1)); hole search for AFilter and
  //                T_ALLOK
  //  HoleSearcher HS(AFilter, T_ALLOK, 0);
  // HoleCollection x = HS.getNext();
  // std::ofstream("tmp.out") << x.to_str() << '\n';
}
