#pragma once

#include <QWidget>
#include "Hole.h"
#include "ui_HoleBrowser.h"

class HoleBrowser : public QWidget {
  Q_OBJECT

 public:
  HoleBrowser(QWidget* parent = nullptr);
  ~HoleBrowser();
  void setHole(Hole* hole);

 private:
  Ui::HoleBrowserClass ui;
};
