#pragma once

#include <QWidget>
#include <QtNetwork>
#include "Hole.h"
#include "ui_HoleBrowser.h"

class HoleBrowser : public QWidget {
  Q_OBJECT

 public:
  HoleBrowser(QWidget* parent = nullptr);
  ~HoleBrowser();
  void setHole(Hole* hole);

 private:
  QNetworkAccessManager networkManager;
  Ui::HoleBrowserClass ui;

 public slots:
  void loadPicture(QNetworkReply* reply);
};
