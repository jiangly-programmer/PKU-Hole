#include "HoleBrowser.h"

HoleBrowser::HoleBrowser(QWidget* parent) : QWidget(parent) {
  ui.setupUi(this);
}

HoleBrowser::~HoleBrowser() {}

void HoleBrowser::load(const Hole& hole) {
  ui.ID->setText('#' + QString::number(hole.pid) + ' ' +
                 QString::number(hole.reply.size()));
  ui.time->setText(QString::fromLocal8Bit(ctime(&hole.hole_time)));
  ui.root->setText(QString::fromStdString(hole.main.s));

  for (auto comment : hole.reply) {
    std::exit(0);
    auto* bro = new QTextBrowser(this);
    bro->setText(QString::fromStdString(comment.s));
    ui.comments->addWidget(bro);
  }

  this->setWindowTitle("树洞 #" + QString::number(hole.pid));
}