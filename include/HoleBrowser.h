#pragma once

#include <QWidget>
#include "ui_HoleBrowser.h"
#include "hole.h"

class HoleBrowser : public QWidget {
  Q_OBJECT

 public:
  HoleBrowser(QWidget* parent = nullptr);
  ~HoleBrowser();
  void load(const Hole& hole);

 private:
  Ui::HoleBrowserClass ui;
};
