#include "quran_reciter.h"
#include "ui_quran_reciter.h"

#include <QDir>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QUrl>
#include <QSlider>
#include <QObject>

#include "core/logging/logging.h"
#include "core/utils/memory.h"
#include "core/constants.h"
#include "core/settings_loader.h"
#include "core/data/data_holder.h"
#include "core/utils/datetime.h"
#include "core/utils/filesystem.h"

QuranReciter::QuranReciter(quran::Quran* quran, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuranReciter),
    m_quran(quran),
    m_currentChapter(nullptr),
    m_ok(false),
    m_mediaPlayer(nullptr),
    m_playList(nullptr)
{
    _TRACE;
    ui->setupUi(this);
    if (m_quran != nullptr) {
        if (m_quran->chapters().size() != quran::Quran::kChapterCount) {
            ui->cboChapter->setEnabled(false);
            ui->spnVerse->setEnabled(false);
            ui->spnVerseFrom->setEnabled(false);
            ui->spnVerseTo->setEnabled(false);
        }
        // Chapters
        for (quran::SingleChapter c : m_quran->chapters()) {
            QString chapterItem = QString::number(static_cast<int>(c.second.name())) + ". " + 
                    QString::fromStdWString(c.second.arabicName()) + 
                    " (" + QString::fromStdWString(c.second.arabicScriptName()) + ")";
            ui->cboChapter->addItem(chapterItem);
        }
        ui->cboChapter->setCurrentIndex(0);
        
        // Reciters
        loadReciters();
        
        // Media player
        if (m_ok) {
            m_mediaPlayer = new QMediaPlayer(this);
            m_playList = new QMediaPlaylist(this);
            m_mediaPlayer->setPlaylist(m_playList);
            
            // Volume
            int kDefaultVolume = 5;
            bool volConvertedOk;
            int v = SettingsLoader::getInstance().get(SettingsLoader::kSettingKeyRecitationVolume, QVariant(kDefaultVolume)).toInt(&volConvertedOk);
            m_mediaPlayer->setVolume(volConvertedOk ? v : kDefaultVolume);
            ui->volSlider->setValue(m_mediaPlayer->volume());
            m_mediaPlayer->setObjectName("m_mediaPlayer");
            connect(ui->volSlider, SIGNAL(valueChanged(int)), this, SLOT(onVolumeChanged(int)));
            connect(ui->volSlider, SIGNAL(sliderMoved(int)), this, SLOT(onVolumeChanged(int)));
            connect(m_mediaPlayer, SIGNAL(volumeChanged(int)), this, SLOT(onVolumeChanged(int)));
            ui->volSlider->setMaximum(100);
            
            on_cboChapter_currentIndexChanged(0);
            ui->btnStop->setEnabled(false);
            ui->chkRepeat->setChecked(false);
            connect(m_playList, SIGNAL(currentIndexChanged(int)), this, SLOT(onVerseChanged(int)));
            connect(m_mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onMediaStateChanged(QMediaPlayer::State)));
            connect(m_mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(onDurationChanged(qint64)));
            connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));
        } else {
            ui->btnPlayPause->setEnabled(false);
            ui->btnStop->setEnabled(false);
            ui->cboChapter->setEnabled(false);
            ui->chkRepeat->setEnabled(false);
            ui->volSlider->hide();
        }
        ui->btnReplayCurrentVerse->setEnabled(false);
        ui->chkRepeat->setChecked(false);
        on_chkRepeat_clicked(false);
        ui->lblDuration->hide();
        showDurationWhenAvailable();
    }
}

QuranReciter::~QuranReciter()
{
    _TRACE;
    memory::deleteAll(m_mediaPlayer, m_playList, ui);
}

void QuranReciter::hideChapterSelector()
{
    _TRACE;
    ui->cboChapter->hide();
    ui->lblCboChapter->hide();
    onControlsVisibilityToggled();
}

void QuranReciter::showChapterSelector()
{
    _TRACE;
    ui->cboChapter->show();
    ui->lblCboChapter->show();
    onControlsVisibilityToggled();
}

void QuranReciter::hideVerseRangeSelector()
{
    _TRACE;
    ui->spnVerseFrom->hide();
    ui->spnVerseTo->hide();
    ui->lblVerseRange->hide();
    ui->lblVerseRangeSeparator->hide();
    onControlsVisibilityToggled();
}

void QuranReciter::showVerseRangeSelector()
{
    _TRACE;
    ui->spnVerseFrom->show();
    ui->spnVerseTo->show();
    ui->lblVerseRange->show();
    ui->lblVerseRangeSeparator->show();
    onControlsVisibilityToggled();
}

void QuranReciter::hideCurrentVerseSelector()
{
    _TRACE;
    ui->spnVerse->hide();
    ui->lblVerse->hide();
    onControlsVisibilityToggled();
}

void QuranReciter::showCurrentVerseSelector()
{
    ui->spnVerse->show();
    ui->lblVerse->show();
    onControlsVisibilityToggled();
}

void QuranReciter::loadReciters()
{
    // TODO: 1.1 Add support to load reciters (keep chapter-by-chapter in mind) from remote server
    // TODO: 2.1 Add support to load reciters chapter-by-chapter
    ui->cboReciter->clear();
    ui->cboReciter->setEnabled(true);
    // Reciters
    const QString noReciterAvailableText = " -- NO RECITER AVAILABLE -- ";
    m_recitationsDir = QDir(filesystem::buildPath(QStringList() 
                                                  << SettingsLoader::getInstance().defaultHomeDir() 
                                                  << "data" << "recitations" << "verse-by-verse"), 
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
            QString reciterDir(filesystem::buildPath(QStringList() 
                                                     << m_recitationsDir.absolutePath()
                                                     << reciter));
            
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
            DVLOG(8) << "Reciter info: " << info.toStdList();
            ui->cboReciter->addItem(info.at(0), QVariant(reciterDir));
            m_ok = true;
        }
    }
    m_ok = m_ok && ui->cboReciter->count() > 0 && ui->cboReciter->isEnabled();
    if (!m_ok && ui->cboReciter->count() == 0) {
        ui->cboReciter->addItem(noReciterAvailableText);
        ui->cboReciter->setEnabled(false);
    }
}

void QuranReciter::stopIfPlaying()
{
    _TRACE;
    if (!m_ok) return;
    on_btnStop_clicked();
}

void QuranReciter::hideDurationWhenAvailable()
{
    _TRACE;
    ui->lblDuration->hide();
    m_hideDuration = true;
}

void QuranReciter::showDurationWhenAvailable()
{
    _TRACE;
    if (m_mediaPlayer != nullptr && m_mediaPlayer->position() > 0) {
        ui->lblDuration->show();
    }
    m_hideDuration = false;
}

void QuranReciter::changeChapter(quran::Chapter::Name chapter)
{
    _TRACE;
    if (!m_ok) return;
    ui->cboChapter->setCurrentIndex(static_cast<int>(chapter) - 1);
}

void QuranReciter::changeVerseRange(int from, int to)
{
    _TRACE;
    if (!m_ok) return;
    if (ui->spnVerseTo->minimum() == from && ui->spnVerseFrom->maximum() == to) {
        return;
    }
    DVLOG(8) << "Changing range [" << from << " - " << to << "]";
    int currVal = ui->spnVerse->value();
    ui->spnVerse->setMinimum(from);
    ui->spnVerse->setMaximum(to);
    if (currVal < from || currVal > to) {
        ui->spnVerse->setValue(from);
        currVal = from;
    }
    if (m_ok) {
        bool playAfterwards = m_mediaPlayer->state() == m_mediaPlayer->PlayingState;
        m_mediaPlayer->stop();
        m_playList->setCurrentIndex(from == 1 ? 0 : currVal);
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

void QuranReciter::changeVerse(int verseNumber)
{
    _TRACE;
    if (!m_ok) return;
    ui->spnVerse->setValue(verseNumber);
}

const quran::Chapter* QuranReciter::currentChapter() const
{
    _TRACE;
    if (!m_ok) return nullptr;
    return m_currentChapter;
}

const quran::Verse* QuranReciter::currentVerse() const
{
    _TRACE;
    if (!m_ok) return nullptr;
    const quran::Chapter* currentChapt = currentChapter();
    if (currentChapt == nullptr) {
        return nullptr;
    }
    return currentChapt->verse(m_playList->currentIndex());
}

void QuranReciter::on_cboChapter_currentIndexChanged(int index)
{
    _TRACE;
    if (!m_ok) return;
    QMediaPlayer::State resumeState = QMediaPlayer::StoppedState;
    int chapterId = index + 1;
    bool isChapterChanged = m_currentChapter != nullptr && 
            static_cast<quran::Chapter::Name>(chapterId) != m_currentChapter->name();
    if (m_mediaPlayer->state() != m_mediaPlayer->PlayingState &&
        m_mediaPlayer->state() != m_mediaPlayer->PausedState) {
        m_mediaPlayer->stop();
    } else {
        // This is case when chapter index is not changed instead it's same chapter
        // but slot is triggered manually
        if (!isChapterChanged) {
            resumeState = m_mediaPlayer->state();
            m_mediaPlayer->pause();
        }
    }
    int resetIndex = isChapterChanged ? 0 : ui->spnVerse->value();
    m_playList->clear();
    
    QString zeroPaddedId;
    if (chapterId < 10) {
        zeroPaddedId.append("00");
    } else if (chapterId < 100) {
        zeroPaddedId.append("0");
    }
    zeroPaddedId.append(QString::number(chapterId));
    QString zeroFile = zeroPaddedId + "000.mp3";
    if (isChapterChanged || m_currentChapter == nullptr) {
        const quran::Chapter* chapter = m_quran->chapter(static_cast<quran::Chapter::Name>(chapterId));
        m_currentChapter = const_cast<quran::Chapter*>(chapter);
        ui->spnVerseFrom->setMaximum(chapter->versesCount());
        ui->spnVerseTo->setMaximum(chapter->versesCount());
        ui->spnVerseTo->setValue(chapter->versesCount());
        
        ui->spnVerse->setMinimum(1);
        ui->spnVerse->setMaximum(chapter->versesCount());
        ui->spnVerse->setValue(1);
    }
    QString recitationFolder = ui->cboReciter->itemData(ui->cboReciter->currentIndex()).toString();
    DVLOG(8) << "Loading recitation data from [" << recitationFolder << "]";
    m_currentRecitationDir = QDir(recitationFolder, zeroPaddedId, 
                                  QDir::Name | QDir::IgnoreCase, 
                                  QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList entries = m_currentRecitationDir.entryList();
    if (entries.isEmpty()) {
        DLOG(ERROR) << "Chapters not found [" << m_currentRecitationDir.absolutePath() << "]";
        m_ok = false;
    } else {
        for (QString chapterFolder : entries) {
            DVLOG(8) << "Loading chapter [" << chapterFolder << "]";
            QDir chapterDir(filesystem::buildPath(QStringList() 
                                                  << m_currentRecitationDir.absolutePath()
                                                  << chapterFolder), "*.mp3", 
                            QDir::Name | QDir::IgnoreCase, 
                            QDir::Files | QDir::NoDotAndDotDot);
            QStringList verseEntries = chapterDir.entryList();
            if (verseEntries.isEmpty()) {
                DVLOG(8) << "Chapter verses not found for [" << chapterFolder << "]";
            } else {
                QString bismillah = recitationFolder + "bismillah.mp3";
                QFileInfo fileInfo(bismillah);
                QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
                m_playList->addMedia(url);
                for (QString verseEntry : verseEntries) {
                    if (verseEntry == zeroFile) {
                        // Zero file = some reciters have bismilliah/audubillah in chapter file
                        // we added it already
                        continue;
                    }
                    // TODO: 1.2 we will need to update this to load media accordingly
                    // TODO: 2.2. This will not be verse entry anymore, it's chapter entry
                    QString fullVersePath = filesystem::buildFilename(QStringList() 
                                                                      << recitationFolder
                                                                      << chapterFolder
                                                                      << verseEntry);
                    QFileInfo fileInfo(fullVersePath);
                    QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
                    m_playList->addMedia(url);
                }
            }
        }
    }
    if (isChapterChanged) {
        emit chapterChanged(m_currentChapter);
    }
    if (!m_playList->isEmpty()) {
        m_playList->setCurrentIndex(resetIndex);
    }
    if (resumeState == QMediaPlayer::PlayingState) {
        m_mediaPlayer->play();
    } else if (resumeState == QMediaPlayer::PausedState) {
        m_mediaPlayer->pause();
    }
}

void QuranReciter::on_btnPlayPause_clicked()
{
    _TRACE;
    if (!m_ok) return;
    if (m_mediaPlayer->state() == QMediaPlayer::PlayingState) {
        m_mediaPlayer->pause();
    } else {
        m_mediaPlayer->play();
    }
}

void QuranReciter::on_btnStop_clicked()
{
    _TRACE;
    if (!m_ok) return;
    m_mediaPlayer->stop();
}

void QuranReciter::on_spnVerseFrom_valueChanged(int)
{
    _TRACE;
    if (!m_ok) return;
    changeVerseRange(ui->spnVerseFrom->value(), ui->spnVerseTo->value());
}

void QuranReciter::on_spnVerseTo_valueChanged(int)
{
    _TRACE;
    if (!m_ok) return;
    changeVerseRange(ui->spnVerseFrom->value(), ui->spnVerseTo->value());
}

void QuranReciter::onVerseChanged(int)
{
    _TRACE;
    if (!m_ok) return;
    if (m_mediaPlayer->state() != QMediaPlayer::PlayingState) {
        return;
    }
    if (m_playList->currentIndex() == -1 || 
        m_playList->currentIndex() > ui->spnVerseTo->value()) {
        m_playList->setCurrentIndex(ui->spnVerseFrom->value());
        if (ui->chkRepeat->isChecked() && ui->spnRepeat->value() > 1) {
            ui->spnRepeat->setValue(ui->spnRepeat->value() - 1);
            DVLOG(7) << "Repeating x " << ui->spnRepeat->value();
            if (ui->spnRepeat->value() == ui->spnRepeat->minimum()) {
                ui->chkRepeat->setChecked(false);
            }
        } else {
            m_mediaPlayer->stop();
        }
    }
    DVLOG(7) << "Reciting verse [" << static_cast<int>(m_currentChapter->name()) << ":" << m_playList->currentIndex() << "]";
    if (m_playList->currentIndex() > 0) {
    
        // TODO: 2.3. we will need to take this into consideration
        //          as we should not be emitting this signal anymore because verse not changed anymore!
        emit currentVerseChanged(m_playList->currentIndex());
        ui->spnVerse->setValue(m_playList->currentIndex());
    }
}

void QuranReciter::onVolumeChanged(int v)
{
    _TRACE;
    if (m_mediaPlayer != nullptr) {
        m_mediaPlayer->setVolume(v);
        SettingsLoader::getInstance().saveSettings(SettingsLoader::kSettingKeyRecitationVolume, QVariant(v));
    }
    ui->volSlider->setValue(v);
}

void QuranReciter::on_chkRepeat_clicked(bool checked)
{
    _TRACE;
    ui->spnRepeat->setEnabled(checked);
}

void QuranReciter::onMediaStateChanged(QMediaPlayer::State state)
{
    _TRACE;
    if (!m_ok) return;
    switch (state) {
    case QMediaPlayer::PlayingState:
        ui->btnPlayPause->setText("▮▮");
        ui->btnStop->setEnabled(true);
        ui->btnReplayCurrentVerse->setEnabled(true);
        if (!m_hideDuration) {
            ui->lblDuration->show();
        }
        DVLOG(7) << "Playing verse [" << m_playList->currentIndex() << "]";
        break;
    case QMediaPlayer::PausedState:
        ui->btnPlayPause->setText("▶");
        ui->btnStop->setEnabled(true);
        ui->btnReplayCurrentVerse->setEnabled(true);
        if (!m_hideDuration) {
            ui->lblDuration->show();
        }
        break;
    case QMediaPlayer::StoppedState:
        ui->btnPlayPause->setText("▶");
        ui->btnStop->setEnabled(false);
        ui->btnReplayCurrentVerse->setEnabled(false);
        ui->lblDuration->hide();
        DVLOG(7) << "Player stopped";
        break;
    }
}

void QuranReciter::on_spnVerse_valueChanged(int index)
{
    _TRACE;
    if (!m_ok) return;
    m_playList->setCurrentIndex(index);
    if (m_mediaPlayer->state() != QMediaPlayer::PlayingState) {
        emit currentVerseChanged(m_playList->currentIndex());
    }
}

void QuranReciter::on_btnReplayCurrentVerse_clicked()
{
    _TRACE;
    if (!m_ok) return;
    if (m_mediaPlayer->state() == QMediaPlayer::PlayingState
        || m_mediaPlayer->state() == QMediaPlayer::PausedState) {
        DVLOG(7) << "Re-reciting verse [" << m_playList->currentIndex() << "]";
        m_mediaPlayer->setPosition(0);
    }
}

void QuranReciter::on_cboReciter_currentIndexChanged(int)
{
    _TRACE;
    if (m_currentChapter != nullptr && ui->cboChapter->count() == quran::Quran::kChapterCount) {
        on_cboChapter_currentIndexChanged(static_cast<int>(m_currentChapter->name()) - 1);
    }
}

void QuranReciter::on_btnReloadReciters_clicked()
{
    _TRACE;
    ui->btnReloadReciters->setEnabled(false);
    loadReciters();
    ui->btnReloadReciters->setEnabled(true);
}

void QuranReciter::onDurationChanged(qint64)
{
    if (!m_ok) return;
    ui->lblDuration->setText(datetime::formatMillisecondsAsDuration(m_mediaPlayer->position()) 
                             + " / " + datetime::formatMillisecondsAsDuration(m_mediaPlayer->duration()));
}

void QuranReciter::onPositionChanged(qint64)
{
    if (!m_ok) return;
    ui->lblDuration->setText(datetime::formatMillisecondsAsDuration(m_mediaPlayer->position()) 
                             + " / " + datetime::formatMillisecondsAsDuration(m_mediaPlayer->duration()));
}


void QuranReciter::onControlsVisibilityToggled()
{
    if (!ui->cboChapter->isVisible() 
        && !ui->spnVerseFrom->isVisible() 
        && !ui->spnVerseTo->isVisible()
        && !ui->spnVerse->isVisible()) {
        resize(width(), 123);
    } else {
        resize(width(), 166);
    }
}
