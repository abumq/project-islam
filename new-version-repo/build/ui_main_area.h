/********************************************************************************
** Form generated from reading UI file 'main_area.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_AREA_H
#define UI_MAIN_AREA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainArea
{
public:
    QLabel *lblTitle;
    QLabel *lblLine;

    void setupUi(QWidget *MainArea)
    {
        if (MainArea->objectName().isEmpty())
            MainArea->setObjectName(QStringLiteral("MainArea"));
        MainArea->resize(1020, 657);
        lblTitle = new QLabel(MainArea);
        lblTitle->setObjectName(QStringLiteral("lblTitle"));
        lblTitle->setGeometry(QRect(10, 10, 681, 31));
        QFont font;
        font.setFamily(QStringLiteral("Abyssinica SIL"));
        font.setPointSize(16);
        lblTitle->setFont(font);
        lblLine = new QLabel(MainArea);
        lblLine->setObjectName(QStringLiteral("lblLine"));
        lblLine->setGeometry(QRect(0, 30, 200000, 17));
        lblLine->setMinimumSize(QSize(200000, 0));
        lblLine->setStyleSheet(QStringLiteral(""));

        retranslateUi(MainArea);

        QMetaObject::connectSlotsByName(MainArea);
    } // setupUi

    void retranslateUi(QWidget *MainArea)
    {
        MainArea->setWindowTitle(QApplication::translate("MainArea", "Form", 0));
        lblTitle->setText(QApplication::translate("MainArea", "Title", 0));
        lblLine->setText(QApplication::translate("MainArea", "<hr>", 0));
    } // retranslateUi

};

namespace Ui {
    class MainArea: public Ui_MainArea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_AREA_H
