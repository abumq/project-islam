/********************************************************************************
** Form generated from reading UI file 'quran_memorization.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QURAN_MEMORIZATION_H
#define UI_QURAN_MEMORIZATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QuranMemorization
{
public:
    QProgressBar *progressBar;

    void setupUi(QWidget *QuranMemorization)
    {
        if (QuranMemorization->objectName().isEmpty())
            QuranMemorization->setObjectName(QStringLiteral("QuranMemorization"));
        QuranMemorization->resize(1011, 694);
        QuranMemorization->setWindowOpacity(1);
        progressBar = new QProgressBar(QuranMemorization);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(110, 70, 381, 181));
        progressBar->setValue(24);

        retranslateUi(QuranMemorization);

        QMetaObject::connectSlotsByName(QuranMemorization);
    } // setupUi

    void retranslateUi(QWidget *QuranMemorization)
    {
        QuranMemorization->setWindowTitle(QApplication::translate("QuranMemorization", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class QuranMemorization: public Ui_QuranMemorization {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QURAN_MEMORIZATION_H
