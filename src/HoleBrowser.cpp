#include "HoleBrowser.h"
#include "HoleCollection.h"

HoleBrowser::HoleBrowser(QWidget* parent) : QWidget(parent) {
  ui.setupUi(this);

  connect(&networkManager, &QNetworkAccessManager::finished, this,
          &HoleBrowser::loadPicture);
}

HoleBrowser::~HoleBrowser() {}

void HoleBrowser::setHole(Hole *hole) {
  ui.ID->setText('#' + QString::number(hole->pid));
  ui.time->setText(QString::fromLocal8Bit(ctime(&hole->hole_time)));
  ui.replys->setText(QString::number(hole->replynum) + "💬");
  ui.likes->setText(QString::number(hole->likenum) + "☆");

  ui.root->setText(QString::fromStdString(hole->main.s));

  if (!hole->img.url.empty()) {
    // TODO: 加载图片
    // ui.root->setText(QString::fromStdString(CONFIG::url + hole.img.url));

    // QNetworkRequest request;
    // request.setUrl(QUrl(QString::fromStdString(hole.img.url)));
    // networkManager.get(request);
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

void HoleBrowser::loadPicture(QNetworkReply* reply) {
  if (reply->error() == QNetworkReply::NoError) {
    QByteArray bytes = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(bytes);

    auto* pic = new QLabel(this);
    pic->setText("12345");
    // ui.label->setPixmap(pixmap);

    ui.verticalLayout->addWidget(pic);
  } else {
    std::exit(0);
  }
  reply->deleteLater();
}