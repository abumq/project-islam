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
    void changeVerse(int);
    const quran::Chapter* currentChapter() const;
    const quran::Verse* currentVerse() const;
    void hideChapterSelector();
    void showChapterSelector();
    void hideVerseRangeSelector();
    void showVerseRangeSelector();
    void hideCurrentVerseSelector();
    void showCurrentVerseSelector();
    void loadReciters();
    void stopIfPlaying();
    void hideDurationWhenAvailable();
    void showDurationWhenAvailable();
private slots:
    void on_cboChapter_currentIndexChanged(int index);
    
    void on_btnPlayPause_clicked();
    
    void on_btnStop_clicked();
    
    void on_spnVerseFrom_valueChanged(int);
    
    void on_spnVerseTo_valueChanged(int);
    
    void onVerseChanged(int);
    
    void onVolumeChanged(int);
    
    void on_chkRepeat_clicked(bool checked);
    
    void onMediaStateChanged(QMediaPlayer::State);
    void on_spnVerse_valueChanged(int);
    
    void on_btnReplayCurrentVerse_clicked();
    
    void on_cboReciter_currentIndexChanged(int index);
    
    void on_btnReloadReciters_clicked();
    void onDurationChanged(qint64);
    void onPositionChanged(qint64);
signals:
    void chapterChanged(const quran::Chapter*);
    void verseRangeChanged(int, int);
    void currentVerseChanged(int);
private:
    Ui::QuranReciter *ui;
    quran::Quran* m_quran;
    quran::Chapter* m_currentChapter;
    QDir m_recitationsDir;
    bool m_ok;
    QDir m_currentRecitationDir;
    QMediaPlayer* m_mediaPlayer;
    QMediaPlaylist* m_playList;
    bool m_hideDuration;
};

#endif // QURAN_RECITER_H
