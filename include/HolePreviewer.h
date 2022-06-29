#pragma once

#include <QWidget>
#include "Hole.h"
#include "ui_HolePreviewer.h"

class HolePreviewer : public QWidget {
  Q_OBJECT

 public:
  HolePreviewer(QWidget* parent = nullptr);
  ~HolePreviewer();
  void setHole(Hole* hole);

 private:
  Hole* hole;
  Ui::HolePreviewerClass ui;

 public slots:
  void browseFull();
};
