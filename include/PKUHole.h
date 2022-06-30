#pragma once

#include "HoleBrowser.h"
#include "HoleCollection.h"
#include "HoleSearcher.h"
#include "ui_PKUHole.h"

class PKUHole : public QMainWindow {
  Q_OBJECT

 public:
  PKUHole(QWidget* parent = nullptr);
  ~PKUHole();

 private:
  int pid;
  HoleSearcher* searcher;
  int batchCount;
  Ui::PKUHoleClass ui;

 public slots:
  void onSearchButtonClicked();
  void loadMore();
};