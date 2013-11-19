/********************************************************************************
** Form generated from reading UI file 'sidebar.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIDEBAR_H
#define UI_SIDEBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Sidebar
{
public:
    QGridLayout *gridLayout_2;
    QTabWidget *sidebarTab;
    QWidget *tabExtensions;
    QGridLayout *gridLayout;
    QListWidget *listFeatures;
    QWidget *tabSettings;

    void setupUi(QWidget *Sidebar)
    {
        if (Sidebar->objectName().isEmpty())
            Sidebar->setObjectName(QStringLiteral("Sidebar"));
        Sidebar->resize(385, 594);
        gridLayout_2 = new QGridLayout(Sidebar);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        sidebarTab = new QTabWidget(Sidebar);
        sidebarTab->setObjectName(QStringLiteral("sidebarTab"));
        sidebarTab->setTabPosition(QTabWidget::South);
        tabExtensions = new QWidget();
        tabExtensions->setObjectName(QStringLiteral("tabExtensions"));
        gridLayout = new QGridLayout(tabExtensions);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        listFeatures = new QListWidget(tabExtensions);
        listFeatures->setObjectName(QStringLiteral("listFeatures"));

        gridLayout->addWidget(listFeatures, 0, 0, 1, 1);

        sidebarTab->addTab(tabExtensions, QString());
        tabSettings = new QWidget();
        tabSettings->setObjectName(QStringLiteral("tabSettings"));
        sidebarTab->addTab(tabSettings, QString());

        gridLayout_2->addWidget(sidebarTab, 0, 0, 1, 1);


        retranslateUi(Sidebar);

        sidebarTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Sidebar);
    } // setupUi

    void retranslateUi(QWidget *Sidebar)
    {
        Sidebar->setWindowTitle(QApplication::translate("Sidebar", "Form", 0));
        sidebarTab->setTabText(sidebarTab->indexOf(tabExtensions), QApplication::translate("Sidebar", "Extensions", 0));
        sidebarTab->setTabText(sidebarTab->indexOf(tabSettings), QApplication::translate("Sidebar", "Settings", 0));
    } // retranslateUi

};

namespace Ui {
    class Sidebar: public Ui_Sidebar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIDEBAR_H
