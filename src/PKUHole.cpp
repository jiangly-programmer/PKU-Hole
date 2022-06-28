#include "PKUHole.h"

PKUHole::PKUHole(QWidget* parent) : QMainWindow(parent) {
  ui.setupUi(this);
}

void PKUHole::jump() {
  int holeID = this->ui.IDEdit->text().toInt();

  auto hole = Hole(API.getone(holeID), HoleJSON::GETONE);

  auto* holeBrowser = new HoleBrowser();
  holeBrowser->load(hole);

  holeBrowser->show();
}

PKUHole::~PKUHole() {}
