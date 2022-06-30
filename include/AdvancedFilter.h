#pragma once

#include <QMainWindow>
#include "ui_AdvancedFilter.h"
#include "Filter.h"

class AdvancedFilter : public QWidget {
  Q_OBJECT

 public:
  AdvancedFilter(QWidget* parent = nullptr);
  ~AdvancedFilter();
  std::pair<int, Filter *> getFilter();
  constexpr static int NoTypeException = 1;
  constexpr static int EmptyKeywordException = 2;

 private:
  Ui::AdvancedFilterClass ui;

 public slots:
  void onTypeChanged(int type);
};
