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
    QuranReader(quran::Quran* quran, quran::Quran* quranTranslation = nullptr, 
            quran::Quran* quranTransliteration = nullptr, QWidget *parent = 0);
    ~QuranReader();
    void changeChapter(quran::Chapter::Name chapterName);
    void changeVerseRange(int from, int to);
    void highlightVerse(int);
    int currentVerseNumber() const;
    void hideChapterSelector();
    void showChapterSelector();
    void hideVerseRangeSelector();
    void showVerseRangeSelector();
    void hideCurrentVerseSelector();
    void showCurrentVerseSelector();
    
    void turnOffTranslation();
    void turnOnTranslation();
    
    void turnOffTransliteration();
    void turnOnTransliteration();
    
private slots:
    void on_cboChapter_currentIndexChanged(int index);
    
    void on_spnVerseFrom_valueChanged(int);
    
    void on_spnVerseTo_valueChanged(int);
    
    void on_spnVerse_valueChanged(int);
    void on_btnMoreControls_clicked(bool checked);
    
    void on_btnZoomOut_clicked();
    
    void on_btnZoomIn_clicked();
    
    void on_chkTransliteration_clicked(bool checked);
    
    void on_chkTranslation_clicked(bool checked);
    
signals:
    void chapterChanged(const quran::Chapter*);
    void verseRangeChanged(int, int);
    void currentVerseChanged(int);
protected:
    void resizeEvent(QResizeEvent *);
private:
    Ui::QuranReader *ui;
    quran::Quran* m_quran;
    quran::Quran* m_quranTranslation;
    quran::Quran* m_quranTransliteration;
    QuranView* m_quranView;
};

#endif // QURAN_READER_H
