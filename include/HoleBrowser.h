#pragma once

#include <QWidget>
#include "hole.h"
#include "ui_HoleBrowser.h"

class HoleBrowser : public QWidget {
  Q_OBJECT

 public:
  HoleBrowser(QWidget* parent = nullptr);
  ~HoleBrowser();
  void load(int holeID);

 private:
  Ui::HoleBrowserClass ui;
};
