#include "PKUHole.h"
#include "HolePreviewer.h"
#include "HoleSearcher.h"

PKUHole::PKUHole(QWidget* parent) : QMainWindow(parent) {
  searcher = nullptr;
  batchCount = 0;
  ui.setupUi(this);

  ui.holeListContents->setMinimumWidth(
      ui.holeListContents->minimumSizeHint().width());

  loadMore();
}

void PKUHole::onSearchButtonClicked() {
  auto text = this->ui.IDEdit->text();

  if (searcher != nullptr) {
    delete searcher;
  }
  if (text.isEmpty()) {
    searcher = nullptr;
  } else {
     searcher = new HoleSearcher(KeywordFilter(text.toStdString(), 1), T_ALLOK, false);
  }
  batchCount = 0;

  while (ui.holeListLayout->count() > 1) {
    auto* hole = ui.holeListLayout->takeAt(0)->widget();
    hole->setVisible(false);
    ui.holeListLayout->removeWidget(hole);
    hole->deleteLater();
  }
  ui.loadMoreButton->setText("加载更多");
  ui.loadMoreButton->setEnabled(true);

  loadMore();
}

void PKUHole::loadMore() {
  HoleCollection holeCollection;
  if (searcher == nullptr) {
    holeCollection =
        HoleCollection::from_getlist_result(API.getlist(++batchCount));
  } else {
    holeCollection = searcher->getNext();
  }
  holeCollection.updateAll();

  if (holeCollection.holes.empty()) {
    ui.loadMoreButton->setText("没有更多了");
    ui.loadMoreButton->setEnabled(false);
  } else {
    for (auto hole : holeCollection.holes) {
      auto* holePreviewer = new HolePreviewer(this);
      holePreviewer->setHole(&hole);
      ui.holeListLayout->insertWidget(ui.holeListLayout->count() - 1,
                                      holePreviewer);
    }
  }
}

PKUHole::~PKUHole() {
  if (searcher != nullptr) {
    delete searcher;
  }
}