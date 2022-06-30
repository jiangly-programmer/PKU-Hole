#include "PKUHole.h"
#include "HolePreviewer.h"
#include "HoleSearcher.h"

PKUHole::PKUHole(QWidget* parent) : QMainWindow(parent) {
  ui.setupUi(this);

  ui.holeListContents->setMinimumWidth(ui.holeListContents->minimumSizeHint().width());

  loadMore();
}

void PKUHole::jump() {
  int holeID = this->ui.IDEdit->text().toInt();

  HoleCollection holeCollection({holeID});
  holeCollection.updateAll();
  auto hole = holeCollection.holes[0];

  auto holeBrowser = new HoleBrowser();
  holeBrowser->setHole(&hole);
  holeBrowser->resize(1920, 1080);

  holeBrowser->show();
}

void PKUHole::loadMore() {
  auto holeCollection = HoleCollection::from_getlist_result(API.getlist(1));
  holeCollection.updateAll();

  for (auto hole : holeCollection.holes) {
    auto* holePreviewer = new HolePreviewer(this);
    holePreviewer->setHole(&hole);
    ui.holeListLayout->insertWidget(ui.holeListLayout->count() - 1, holePreviewer);
  }
}

PKUHole::~PKUHole() {}