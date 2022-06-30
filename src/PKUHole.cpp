#include "PKUHole.h"
#include "HolePreviewer.h"
#include "HoleSearcher.h"
#include <QScrollBar>

PKUHole::PKUHole(QWidget* parent)
    : QMainWindow(parent), pid(-1), searcher(nullptr), batchCount(0) {
  ui.setupUi(this);

  ui.holeListContents->setMinimumWidth(
      ui.holeListContents->minimumSizeHint().width());

  loadMore();
}

void PKUHole::onSearchButtonClicked() {
  auto text = this->ui.IDEdit->text();
  
  pid = -1;

  bool ok = false;
  if (text[0] == '#' && text.mid(1).toInt(&ok) && ok) {
    pid = text.mid(1).toInt();
  }
  if (text.toInt(&ok) && ok) {
    pid = text.toInt();
  }

  if (searcher != nullptr) {
    delete searcher;
  }
  if (text.isEmpty()) {
    searcher = nullptr;
  } else {
    searcher =
        new HoleSearcher(KeywordFilter(text.toStdString(), 1), T_ALLOK, false);
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
  ui.holeList->verticalScrollBar()->setValue(0);

  loadMore();
}

void PKUHole::loadMore() {
  HoleCollection holeCollection;
  if (searcher == nullptr) {
    holeCollection =
        HoleCollection::from_getlist_result(API.getlist(++batchCount));
  } else {
    holeCollection = searcher->getNext();
    batchCount++;
  }
  holeCollection.updateAll();

  if (pid != -1 && batchCount == 1) {
    HoleCollection single({pid});
    single.updateAll();
    for (auto hole : single.holes) {
      holeCollection.holes.insert(holeCollection.begin(), hole);
    }
  }

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