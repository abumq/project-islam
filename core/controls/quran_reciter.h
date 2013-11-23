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

namespace data {
class DataHolder;
}

class QMediaPlaylist;

class QuranReciter : public QWidget
{
    Q_OBJECT
    
public:
    QuranReciter(data::DataHolder* dataHolder, QWidget *parent = 0);
    ~QuranReciter();
    void changeChapter(quran::Chapter::Name chapter);
    void changeVerse(int from, int);
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
    
private:
    Ui::QuranReciter *ui;
    data::DataHolder* m_dataHolder;
    QDir m_recitationsDir;
    bool m_ok;
    QDir m_currentRecitationDir;
    QMediaPlayer* m_mediaPlayer;
    QMediaPlaylist* m_playList;
};

#endif // QURAN_RECITER_H
