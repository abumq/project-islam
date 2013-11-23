#ifndef QURAN_RECITER_H
#define QURAN_RECITER_H

#include <QWidget>
#include <QDir>
#include <QMediaContent>
#include <QMediaPlayer>
#include "core/quran/chapter.h"

namespace Ui {
class QuranReciter;
}

namespace quran {
class Quran;
}

class QMediaPlaylist;

class QuranReciter : public QWidget
{
    Q_OBJECT
    
public:
    QuranReciter(quran::Quran* quran, QWidget *parent = 0);
    ~QuranReciter();
    void changeChapter(quran::Chapter::Name chapter);
    void changeVerseRange(int from, int to);
    const quran::Chapter* currentChapter() const;
    const quran::Verse* currentVerse() const;
private slots:
    void on_cboChapter_currentIndexChanged(int index);
    
    void on_btnPlay_clicked();
    
    void on_btnPause_clicked();
    
    void on_btnStop_clicked();
    
    void on_spnVerseFrom_valueChanged(int);
    
    void on_spnVerseTo_valueChanged(int);
    
    void onVerseChanged(int);
    
    void on_chkRepeat_clicked(bool checked);
    
    void onMediaStateChanged(QMediaPlayer::State);
signals:
    void chapterChanged(const quran::Chapter*);
    void verseRangeChanged(int, int);
private:
    Ui::QuranReciter *ui;
    quran::Quran* m_quran;
    QDir m_recitationsDir;
    bool m_ok;
    QDir m_currentRecitationDir;
    QMediaPlayer* m_mediaPlayer;
    QMediaPlaylist* m_playList;
};

#endif // QURAN_RECITER_H
