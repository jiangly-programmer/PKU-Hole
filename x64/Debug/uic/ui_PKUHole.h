/********************************************************************************
** Form generated from reading UI file 'PKUHole.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PKUHOLE_H
#define UI_PKUHOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PKUHoleClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PKUHoleClass)
    {
        if (PKUHoleClass->objectName().isEmpty())
            PKUHoleClass->setObjectName(QString::fromUtf8("PKUHoleClass"));
        PKUHoleClass->resize(600, 400);
        menuBar = new QMenuBar(PKUHoleClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        PKUHoleClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PKUHoleClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        PKUHoleClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(PKUHoleClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        PKUHoleClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(PKUHoleClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        PKUHoleClass->setStatusBar(statusBar);

        retranslateUi(PKUHoleClass);

        QMetaObject::connectSlotsByName(PKUHoleClass);
    } // setupUi

    void retranslateUi(QMainWindow *PKUHoleClass)
    {
        PKUHoleClass->setWindowTitle(QApplication::translate("PKUHoleClass", "PKUHole", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PKUHoleClass: public Ui_PKUHoleClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PKUHOLE_H
