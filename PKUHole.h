#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PKUHole.h"

class PKUHole : public QMainWindow
{
    Q_OBJECT

public:
    PKUHole(QWidget *parent = nullptr);
    ~PKUHole();

private:
    Ui::PKUHoleClass ui;
};
