#ifndef QURAN_READER_H
#define QURAN_READER_H

#include <QWidget>
#include "core/quran/chapter.h"
namespace Ui {
class QuranReader;
}

namespace quran {
class Quran;
}
class QuranView;

class QuranReader : public QWidget
{
    Q_OBJECT
    
public:
    QuranReader(quran::Quran* quran, QWidget *parent = 0);
    ~QuranReader();
    void changeChapter(quran::Chapter::Name chapterName);
    void changeVerseRange(int from, int to);
    void highlightVerse(int);
    int currentVerse();
private slots:
    void on_cboChapter_currentIndexChanged(int index);
    
    void on_spnVerseFrom_valueChanged(int);
    
    void on_spnVerseTo_valueChanged(int);
    
    void on_spnVerse_valueChanged(int);
signals:
    void chapterChanged(const quran::Chapter*);
    void verseRangeChanged(int, int);
    void selectedVerseChanged(int);
    
private:
    Ui::QuranReader *ui;
    quran::Quran* m_quran;
    QuranView* m_quranView;
};

#endif // QURAN_READER_H
