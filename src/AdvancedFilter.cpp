#include "AdvancedFilter.h"
#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>

void clearLayout(QLayout* layout) {
  if (layout == nullptr) {
    return;
  }

  while (layout->count() > 0) {
    auto* item = layout->takeAt(0);
    if (item->layout()) {
      clearLayout(item->layout());
      item->layout()->deleteLater();
    }
    if (item->widget()) {
      item->widget()->deleteLater();
    }
  }
}

AdvancedFilter::AdvancedFilter(QWidget* parent) : QWidget(parent) {
  ui.setupUi(this);

  ui.filterType->addItem("请选择条件类型……");
  ui.filterType->addItem("[条件 1] 且 [条件 2]");
  ui.filterType->addItem("[条件 1] 或 [条件 2]");
  ui.filterType->addItem("包含 [关键字]");
  ui.filterType->addItem("不包含 [关键字]");
}

std::pair<int, Filter*> AdvancedFilter::getFilter() {
  int type = ui.filterType->currentIndex();

  if (type == 0) {
    return std::pair(NoTypeException, nullptr);
  }

  if (type == 3 || type == 4) {
    auto keyword = ((QLineEdit*)ui.filterArgs->itemAt(1)->widget())->text();
    if (keyword.isEmpty()) {
      return std::pair(EmptyKeywordException, nullptr);
    }

    auto filter =
        new Filter(Filter::KeywordFilterType::Leaf, keyword.toStdString(), 1);
    if (type == 3) {
      return std::pair(0, filter);
    } else {
      return std::pair(0,
                       new Filter(Filter::KeywordFilterType::Not, {*filter}));
    }
  } else {
    auto filter1 = ((AdvancedFilter*)ui.filterArgs->itemAt(0)
                        ->layout()
                        ->itemAt(1)
                        ->widget())
                       ->getFilter();
    auto filter2 = ((AdvancedFilter*)ui.filterArgs->itemAt(2)
                        ->layout()
                        ->itemAt(1)
                        ->widget())
                       ->getFilter();
    if (filter1.first != 0 || filter2.first != 0) {
      return std::pair(filter1.first | filter2.first, nullptr);
    }

    if (type == 1) {
      return std::pair(0, new Filter(Filter::KeywordFilterType::And,
                                     {*filter1.second, *filter2.second}));
    } else {
      return std::pair(0, new Filter(Filter::KeywordFilterType::Or,
                                     {*filter1.second, *filter2.second}));
    }
  }
}

void AdvancedFilter::onTypeChanged(int type) {
  clearLayout(ui.filterArgs);

  if (type == 1 || type == 2) {
    for (int t = 1; t <= 2; t++) {
      auto* ver = new QVBoxLayout(this);

      auto* lbl = new QLabel("条件 " + QString::number(t) + "：");
      lbl->setAlignment(Qt::AlignCenter);
      lbl->setFont(QFont("Microsoft YaHei UI", 12));

      ver->addWidget(lbl);

      ver->addWidget(new AdvancedFilter(this));

      ui.filterArgs->addLayout(ver);

      if (t == 1) {
        auto* line = new QFrame(this);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        ui.filterArgs->addWidget(line);
      }
    }
  } else if (type == 3 || type == 4) {
    auto* lbl = new QLabel("关键字：", this);
    lbl->setFont(QFont("Microsoft YaHei UI", 12));

    ui.filterArgs->addWidget(lbl);

    auto* edit = new QLineEdit(this);
    edit->setFont(QFont("Microsoft YaHei", 12));

    ui.filterArgs->addWidget(edit);
  }
}

AdvancedFilter::~AdvancedFilter() {}
