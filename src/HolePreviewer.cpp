#include "HolePreviewer.h"
#include "HoleBrowser.h"
#include <QPushButton>

HolePreviewer::HolePreviewer(QWidget* parent) : QWidget(parent), hole(nullptr) {
  ui.setupUi(this);
}

void HolePreviewer::setHole(Hole *hole) {
  this->hole = new Hole(*hole);

  ui.ID->setText('#' + QString::number(hole->pid));
  ui.time->setText(QString::fromLocal8Bit(ctime(&hole->hole_time)));
  ui.replys->setText(QString::number(hole->replynum) + "💬");
  ui.likes->setText(QString::number(hole->likenum) + "☆");

  ui.root->setText(QString::fromStdString(hole->main.s));

  for (int i = 0; i < std::min(3, hole->replynum); i++) {
    auto* comment = new QLabel(this);
    comment->setText(QString::fromStdString(hole->reply[i].s));
    comment->setFont(QFont("Microsoft YaHei", 12));
    comment->setStyleSheet("QLabel{background:#FFFFFF;}");
    comment->setWordWrap(true);
    comment->setMargin(5);
    ui.commentPreviewer->addWidget(comment);
  }

  if (hole->replynum > 3) {
    auto* more = new QPushButton(this);
    more->setText("共 " + QString::number(hole->replynum) + " 条回复");
    more->setFont(QFont("Microsoft YaHei", 12));
    more->setStyleSheet("QLabel{background:#FFFFFF;}");
    more->setCursor(Qt::PointingHandCursor);
    ui.commentPreviewer->addWidget(more);

    connect(more, &QPushButton::clicked, this, &HolePreviewer::browseFull);
  }
}

void HolePreviewer::browseFull() {
  auto holeBrowser = new HoleBrowser();
  holeBrowser->setHole(this->hole);
  holeBrowser->resize(1920, 1080);

  holeBrowser->show();
}

HolePreviewer::~HolePreviewer() {
  if (hole != nullptr) {
    delete hole;
  }
}
