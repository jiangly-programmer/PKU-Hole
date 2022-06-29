#include "PKUHole.h"
#include "HoleSearcher.h"
#include "HolePreviewer.h"

PKUHole::PKUHole(QWidget* parent) : QMainWindow(parent) {
  ui.setupUi(this);

  refresh();
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

void PKUHole::refresh() {
  HoleSearcher holeSearcher(ALLOK, T_ALLOK, false);

  auto holeCollection = holeSearcher.getNext();
  holeCollection.updateAll();

  for (auto hole : holeCollection.holes) {
    auto* holePreviewer = new HolePreviewer(this);
    holePreviewer->setHole(&hole);
    ui.holeListLayout->addWidget(holePreviewer);
  }
}

PKUHole::~PKUHole() {}