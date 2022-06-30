#include "AdvancedSearch.h"
#include <QMessageBox>

AdvancedSearch::AdvancedSearch(QWidget* parent) : QWidget(parent) {
  ui.setupUi(this);
}

void AdvancedSearch::onSearchButtonClicked() {
  auto filter = ui.mainFilter->getFilter();

  if (filter.first != 0) {
    QString error;

    if (filter.first & AdvancedFilter::NoTypeException) {
      error += "请选择条件类型！";
    }
    if (filter.first & AdvancedFilter::EmptyKeywordException) {
      error += "关键字不能为空！";
    }
    QMessageBox::critical(this, "错误", error);
  } else {
    advancedSearchConfirmed(filter.second);
    close();
  }
}

AdvancedSearch::~AdvancedSearch() {}
