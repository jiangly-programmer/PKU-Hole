#include "PKUHole.h"

PKUHole::PKUHole(QWidget* parent) : QMainWindow(parent) {
  ui.setupUi(this);
}

void PKUHole::jump() {
  int holeID = this->ui.IDEdit->text().toInt();

  auto holeBrowser = new HoleBrowser();
  holeBrowser->load(holeID);

  holeBrowser->show();
}

PKUHole::~PKUHole() {}
