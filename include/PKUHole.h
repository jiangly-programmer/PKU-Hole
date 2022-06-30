#pragma once

#include "HoleBrowser.h"
#include "HoleCollection.h"
#include "ui_PKUHole.h"

class PKUHole : public QMainWindow {
  Q_OBJECT

 public:
  PKUHole(QWidget* parent = nullptr);
  ~PKUHole();

 private:
  Ui::PKUHoleClass ui;

 public slots:
  void jump();
  void loadMore();
};