#pragma once

#include <QWidget>
#include "ui_AdvancedSearch.h"

class AdvancedSearch : public QWidget {
  Q_OBJECT

 public:
  AdvancedSearch(QWidget* parent = nullptr);
  ~AdvancedSearch();

 private:
  Ui::AdvancedSearchClass ui;

 signals:
  void advancedSearchConfirmed(Filter* filter);

 public slots:
  void onSearchButtonClicked();
};
