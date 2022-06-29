#include "HoleBrowser.h"
#include "holecollection.h"

HoleBrowser::HoleBrowser(QWidget* parent) : QWidget(parent) {
  ui.setupUi(this);
}

HoleBrowser::~HoleBrowser() {}

void HoleBrowser::load(int holeID) {
  auto holeCollection = HoleCollection({holeID});
  holeCollection.updateAll();

  auto hole = holeCollection.holes[0];

  ui.ID->setText('#' + QString::number(hole.pid));
  ui.time->setText(QString::fromLocal8Bit(ctime(&hole.hole_time)));
  ui.root->setText(QString::fromStdString(hole.main.s));

  for (const auto& comment : hole.reply) {
    auto* bro = new QTextBrowser(ui.scrollAreaWidgetContents);
    bro->setText(QString::fromStdString(comment.s));
    bro->setFont(QFont("Microsoft YaHei", 12));
    bro->document()->adjustSize();
    bro->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    bro->setMinimumHeight(bro->document()->size().height());
    ui.comments->addWidget(bro);
  }

  this->setWindowTitle("树洞 #" + QString::number(hole.pid));
}