#include "HoleBrowser.h"
#include "HoleCollection.h"

HoleBrowser::HoleBrowser(QWidget* parent) : QWidget(parent) {
  ui.setupUi(this);
}

HoleBrowser::~HoleBrowser() {}

void HoleBrowser::setHole(Hole *hole) {
  ui.ID->setText('#' + QString::number(hole->pid));
  ui.time->setText(QString::fromLocal8Bit(ctime(&hole->hole_time)));
  ui.replys->setText(QString::number(hole->replynum) + "💬");
  ui.likes->setText(QString::number(hole->likenum) + "☆");

  ui.root->setText(QString::fromStdString(hole->main.s));

  if (!hole->img.url.empty()) {
    auto result = API.getimage("https://pkuhelper.pku.edu.cn/services/pkuhole/images/" + hole->img.url);

    auto bytes = QByteArray::fromStdString(result);

    QPixmap pixmap;
    pixmap.loadFromData(bytes);

    auto* pic = new QLabel(this);
    pic->setPixmap(pixmap.scaled(1000, 1000, Qt::KeepAspectRatio));
    pic->setAlignment(Qt::AlignCenter);

    ui.verticalLayout->insertWidget(2, pic);
  }

  for (const auto& comment : hole->reply) {
    auto* lbl = new QLabel(ui.commentContents);
    lbl->setText(QString::fromStdString(comment.s));
    lbl->setFont(QFont("Microsoft YaHei", 12));
    lbl->setStyleSheet("QLabel{background:#FFFFFF;}");
    lbl->setWordWrap(true);
    lbl->setMargin(5);
    ui.commentContentsLayout->addWidget(lbl);
  }

  this->setWindowTitle("树洞 #" + QString::number(hole->pid));
}
