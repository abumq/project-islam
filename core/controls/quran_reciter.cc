#include "quran_reciter.h"
#include "ui_quran_reciter.h"
#include <QDir>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QUrl>
#include <QObject>
#include "core/data/data_holder.h"
#include "core/logging.h"
#include "core/constants.h"
QuranReciter::QuranReciter(quran::Quran* quran, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuranReciter),
    m_quran(quran),
    m_ok(false),
    m_mediaPlayer(nullptr),
    m_playList(nullptr)
{
    ui->setupUi(this);
    
    // Chapters
    for (quran::SingleChapter c : m_quran->chapters()) {
        QString chapterItem = QString::number(static_cast<int>(c.second.name())) + ". " + 
                QString::fromStdWString(c.second.arabicName()) + 
                " (" + QString::fromStdWString(c.second.arabicScriptName()) + ")";
        ui->cboChapter->addItem(chapterItem);
    }
    ui->cboChapter->setCurrentIndex(0);
    
    // Reciters
    const QString noReciterAvailableText = " -- NO RECITER AVAILABLE -- ";
    m_recitationsDir = QDir(QString::fromStdString(kHomeDir) 
                            + "data" + QDir::separator() + "recitations" + QDir::separator(), 
                            QString(), QDir::Name | QDir::IgnoreCase, QDir::Dirs | QDir::NoDotAndDotDot);
    if (!m_recitationsDir.exists()) {
        DLOG(ERROR) << "Recitations directory [" << m_recitationsDir.absolutePath() << "] not found";
        ui->cboReciter->addItem(noReciterAvailableText);
        ui->cboReciter->setEnabled(false);
    } else {
        QStringList recitersList =  m_recitationsDir.entryList();
        if (recitersList.empty()) {
            DLOG(ERROR) << "No reciter found in [" << m_recitationsDir.absolutePath() << "]";
            ui->cboReciter->addItem(noReciterAvailableText);
            ui->cboReciter->setEnabled(false);
        } else {
            DLOG(INFO) << "Loading reciters...";
        }
        for (QString reciter : recitersList) {
            QString reciterDir(m_recitationsDir.absolutePath() + QDir::separator() + reciter + QDir::separator());
            
            QFile f(reciterDir + "info");
            if (!f.open(QFile::ReadOnly) || !f.exists()) {
                LOG(ERROR) << "Info file not found for [" << reciter << "] in [" << f.fileName() << "]";
                continue;
            }
            QString fileContents = QString(f.readAll());
            QStringList info = fileContents.split(",");
            if (info.empty() || info.at(0).isEmpty()) {
                DLOG(ERROR) << "Invalid info file [" << f.fileName() << "] Contents: " << fileContents;
                continue;
            }
            DLOG(INFO) << "Reciter info: " << info.toStdList();
            ui->cboReciter->addItem(info.at(0), QVariant(reciterDir));
            m_ok = true;
        }
    }
    m_ok = m_ok && ui->cboReciter->count() > 0 && ui->cboReciter->isEnabled();
    if (!m_ok && ui->cboReciter->count() == 0) {
        ui->cboReciter->addItem(noReciterAvailableText);
        ui->cboReciter->setEnabled(false);
    }
    
    // Media player
    if (m_ok) {
        m_mediaPlayer = new QMediaPlayer(this);
        m_playList = new QMediaPlaylist(this);
        m_mediaPlayer->setPlaylist(m_playList);
        on_cboChapter_currentIndexChanged(0);
        ui->btnPause->setEnabled(false);
        ui->btnStop->setEnabled(false);
        ui->chkRepeat->setChecked(false);
        on_chkRepeat_clicked(false);
        connect(m_playList, SIGNAL(currentIndexChanged(int)), this, SLOT(onVerseChanged(int)));
        connect(m_mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onMediaStateChanged(QMediaPlayer::State)));
    } else {
        ui->btnPause->setEnabled(false);
        ui->btnPlay->setEnabled(false);
        ui->btnStop->setEnabled(false);
        ui->cboChapter->setEnabled(false);
    }
}

QuranReciter::~QuranReciter()
{
    delete ui;
}

void QuranReciter::changeChapter(quran::Chapter::Name chapter)
{
    ui->cboChapter->setCurrentIndex(static_cast<int>(chapter) - 1);
}

void QuranReciter::changeVerseRange(int from, int to)
{
    _TRACE;
    if (ui->spnVerseTo->minimum() == from && ui->spnVerseFrom->maximum() == to) {
        return;
    }
    LOG(INFO) << "Changing range [" << from << " - " << to << "]";
    if (m_ok) {
        bool playAfterwards = m_mediaPlayer->state() == m_mediaPlayer->PlayingState;
        m_mediaPlayer->stop();
        m_playList->setCurrentIndex(from);
        if (playAfterwards) {
            m_mediaPlayer->play();
        }
    }
    ui->spnVerseTo->setMinimum(from);
    ui->spnVerseTo->setValue(to);
    ui->spnVerseFrom->setMaximum(to);
    ui->spnVerseFrom->setValue(from);
    
    emit verseRangeChanged(from, to);
}

const quran::Chapter* QuranReciter::currentChapter() const
{
    return m_quran->chapter(static_cast<quran::Chapter::Name>(ui->cboChapter->currentIndex() + 1));
}

const quran::Verse* QuranReciter::currentVerse() const
{
    const quran::Chapter* currentChapt = currentChapter();
    if (currentChapt == nullptr) {
        return nullptr;
    }
    return currentChapt->verse(m_playList->currentIndex());
}

void QuranReciter::on_cboChapter_currentIndexChanged(int index)
{
    if (m_playList == nullptr) return;
    if (m_mediaPlayer->state() == m_mediaPlayer->PlayingState) {
        m_mediaPlayer->stop();
    }
    m_playList->clear();
    
    int chapterId = index + 1;
    QString zeroPaddedId;
    if (chapterId < 10) {
        zeroPaddedId.append("00");
    } else if (chapterId < 100) {
        zeroPaddedId.append("0");
    }
    zeroPaddedId.append(QString::number(chapterId));
    
    const quran::Chapter* chapter = m_quran->chapter(static_cast<quran::Chapter::Name>(chapterId));
    ui->spnVerseFrom->setMaximum(chapter->versesCount());
    ui->spnVerseTo->setMaximum(chapter->versesCount());
    ui->spnVerseTo->setValue(chapter->versesCount());
    
    QString recitationFolder = ui->cboReciter->itemData(ui->cboReciter->currentIndex()).toString();
    DLOG(INFO) << "Loading recitation data from [" << recitationFolder << "]";
    m_currentRecitationDir = QDir(recitationFolder, zeroPaddedId, QDir::Name | QDir::IgnoreCase, QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList entries = m_currentRecitationDir.entryList();
    if (entries.isEmpty()) {
        DLOG(ERROR) << "Chapters not found [" << m_currentRecitationDir.absolutePath() << "]";
        m_ok = false;
    } else {
        for (QString chapterFolder : entries) {
            DLOG(INFO) << "Loading chapter [" << chapterFolder << "]";
            QDir chapterDir(m_currentRecitationDir.absolutePath() + QDir::separator() + chapterFolder, "*.mp3", QDir::Name | QDir::IgnoreCase, QDir::Files | QDir::NoDotAndDotDot);
            QStringList verseEntries = chapterDir.entryList();
            if (verseEntries.isEmpty()) {
                DLOG(ERROR) << "Chapter verses not found for [" << chapterFolder << "]";
            } else {
                for (QString verseEntry : verseEntries) {
                    QString fullVersePath = recitationFolder + chapterFolder + QDir::separator() + verseEntry;
                    QFileInfo fileInfo(fullVersePath);
                    QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
                    m_playList->addMedia(url);
                }
            }
        }
    }
    emit chapterChanged(chapter);
    if (!m_playList->isEmpty()) {
        m_playList->setCurrentIndex(0);
    }
}

void QuranReciter::on_btnPlay_clicked()
{
    if (m_mediaPlayer != nullptr) {
        m_mediaPlayer->play();
    }
}

void QuranReciter::on_btnPause_clicked()
{
    if (m_mediaPlayer != nullptr) {
        m_mediaPlayer->pause();
    }
}

void QuranReciter::on_btnStop_clicked()
{
    if (m_mediaPlayer != nullptr) {
        m_mediaPlayer->stop();
    }
}

void QuranReciter::on_spnVerseFrom_valueChanged(int)
{
    changeVerseRange(ui->spnVerseFrom->value(), ui->spnVerseTo->value());
}

void QuranReciter::on_spnVerseTo_valueChanged(int)
{
    changeVerseRange(ui->spnVerseFrom->value(), ui->spnVerseTo->value());
}

void QuranReciter::onVerseChanged(int)
{
    if (m_mediaPlayer->state() != QMediaPlayer::PlayingState) {
        return;
    }
    if (m_playList->currentIndex() == 0) {
        // Ignore files *000.mp3
        return;
    }
    LOG(INFO) << "Playing verse [" << m_playList->currentIndex() << "]";
    if (m_playList->currentIndex() > ui->spnVerseTo->value()) {
        m_playList->setCurrentIndex(ui->spnVerseFrom->value());
        if (ui->chkRepeat->isChecked()) {
            ui->spnRepeat->setValue(ui->spnRepeat->value() - 1);
            LOG(INFO) << "Repeating x " << ui->spnRepeat->value();
            if (ui->spnRepeat->value() == ui->spnRepeat->minimum()) {
                ui->chkRepeat->setChecked(false);
            }
        } else {
            m_mediaPlayer->stop();
        }
    }
    emit currentVerseChanged(m_playList->currentIndex());
}

void QuranReciter::on_chkRepeat_clicked(bool checked)
{
    ui->spnRepeat->setEnabled(checked);
}

void QuranReciter::onMediaStateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::PlayingState:
        ui->btnPlay->setEnabled(false);
        ui->btnPause->setEnabled(true);
        ui->btnStop->setEnabled(true);
        LOG(INFO) << "Playing verse [" << m_playList->currentIndex() << "]";
        break;
    case QMediaPlayer::PausedState:
        ui->btnPlay->setEnabled(true);
        ui->btnPause->setEnabled(false);
        ui->btnStop->setEnabled(false);
        break;
    case QMediaPlayer::StoppedState:
        ui->btnPlay->setEnabled(true);
        ui->btnPause->setEnabled(false);
        ui->btnStop->setEnabled(false);
        LOG(INFO) << "Stopped";
        break;
    }
}
