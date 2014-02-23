#include "al_quran.h"

#include <QLabel>
#include <QAction>
#include <QApplication>
#include <QSplitter>
#include <QGridLayout>
#include "bookmarks_bar.h"
#include "core/controls/quran_reciter.h"
#include "core/controls/quran_reader.h"

INITIALIZE_SHARED_LOGGING

const char* ExtensionBase::kAuthor       = "Project Islam Authors";
const char* ExtensionBase::kName         = "Al-Quran";
const char* ExtensionBase::kTitle        = "Al-Qur'an";
const char* ExtensionBase::kDescription  = "Al-Qur'an recitation, reading and help memorize yourself.";

AlQuran::AlQuran()
{
    memory::turnToNullPtr(m_reciter, m_reader, m_bookmarkBar, m_splitter, m_rightBar);
    Q_INIT_RESOURCE(icons_al_quran);
}

AlQuran::~AlQuran()
{
    // m_reader, m_reciter not available! deleted on ~container()
}

bool AlQuran::initialize(int argc, const char** argv)
{
    if (!ExtensionBase::initialize(argc, argv)) {
        // Do not proceed
        return false;
    }
    
    // Do not trace location before calling parent's initialize
    _TRACE;
    memory::deleteAll(m_reciter, m_reader, m_bookmarkBar, m_splitter, m_rightBar);
    m_splitter = new QSplitter(container());
    m_splitter->setOrientation(Qt::Horizontal);
    m_reciter = new QuranReciter(data()->quranArabic(), container());
    m_reader = new QuranReader(data()->quranArabic(), 
                               data()->quranTranslation(), 
                               data()->quranTransliteration(),
                               data()->quranTafseer(),
                               m_splitter);
    QObject::connect(m_reciter, SIGNAL(chapterChanged(const quran::Chapter*)), this, SLOT(onChapterChangedReciter(const quran::Chapter*)));
    QObject::connect(m_reciter, SIGNAL(verseRangeChanged(int,int)), this, SLOT(onVerseRangeChangedReciter(int,int)));
    QObject::connect(m_reader, SIGNAL(chapterChanged(const quran::Chapter*)), this, SLOT(onChapterChangedReader(const quran::Chapter*)));
    QObject::connect(m_reader, SIGNAL(verseRangeChanged(int,int)), this, SLOT(onVerseRangeChangedReader(int,int)));
    QObject::connect(m_reader, SIGNAL(currentVerseChanged(int)), this, SLOT(onSelectedVerseChangedReader(int)));
    QObject::connect(m_reader, SIGNAL(translationToggled(bool)), this, SLOT(onToggledTranslation(bool)));
    QObject::connect(m_reader, SIGNAL(transliterationOnToggled(bool)), this, SLOT(onToggledTransliteration(bool)));
    QObject::connect(m_reader, SIGNAL(tafseerToggled(bool)), this, SLOT(onToggledTafseer(bool)));
    QObject::connect(m_reciter, SIGNAL(currentVerseChanged(int)), this, SLOT(onSelectedVerseChangedReciter(int)));
    QObject::connect(m_reader, SIGNAL(jumpToTextChanged(QString)), this, SLOT(onJumpToTextChanged(QString)));
    m_rightBar = new QWidget(m_splitter);
    QGridLayout* rightBarLayout = new QGridLayout(m_rightBar);
    // Bookmarks bar
    m_bookmarkBar = new BookmarksBar(settingsKeyPrefix());
    QObject::connect(m_bookmarkBar, SIGNAL(selectionChanged(Bookmark*)), this, SLOT(onBookmarkChanged(Bookmark*)));
    rightBarLayout->addWidget(m_bookmarkBar);
    
    initializeMenu();
    
    // Translation
    if (setting("load_translation", QVariant(true)).toBool()) {
        m_reader->turnOnTranslation();
    } else {
        m_reader->turnOffTranslation();
    }
    // Transliteration
    if (setting("load_transliteration", QVariant(true)).toBool()) {
        m_reader->turnOnTransliteration();
    } else {
        m_reader->turnOffTransliteration();
    }
    // Transliteration
    if (setting("load_tafseer", QVariant(true)).toBool()) {
        m_reader->turnOnTafseer();
    } else {
        m_reader->turnOffTafseer();
    }
    // Duration
    if (setting("load_recite_duration", QVariant(true)).toBool()) {
        m_reciter->showDurationWhenAvailable();
    } else {
        m_reciter->hideDurationWhenAvailable();
    }
    // Force trigger this slot
    onContainerGeometryChanged(container()->width(), container()->height());
    return true;
}

void AlQuran::initializeMenu()
{
    _TRACE;
    QAction* showReaderAction = extensionMenu()->addAction("Show Reader");
    QObject::connect(showReaderAction, SIGNAL(toggled(bool)), this, SLOT(toggleReader(bool)));
    showReaderAction->setCheckable(true);
    bool reader = setting("show_reader", QVariant(true)).toBool();
    showReaderAction->setChecked(reader);
    toggleReader(reader);
    
    QAction* showReciterAction = extensionMenu()->addAction("Show Reciter");
    QObject::connect(showReciterAction, SIGNAL(toggled(bool)), this, SLOT(toggleReciter(bool)));
    showReciterAction->setCheckable(true);
    bool reciter = setting("show_reciter", QVariant(true)).toBool();
    showReciterAction->setChecked(reciter);
    toggleReciter(reciter);
    
    QAction* showDurationAction = extensionMenu()->addAction("Show Duration");
    showDurationAction->setObjectName("showDurationAction");
    QObject::connect(showDurationAction, SIGNAL(toggled(bool)), this, SLOT(toggleShowDuration(bool)));
    showDurationAction->setCheckable(true);
    bool showDuration = setting("load_recite_duration", QVariant(true)).toBool();
    showDurationAction->setChecked(showDuration);
    toggleShowDuration(showDuration);
    
    QAction* showBookmarksAction = extensionMenu()->addAction("Bookmarks");
    showBookmarksAction->setObjectName("showBookmarksAction");
    QObject::connect(showBookmarksAction, SIGNAL(toggled(bool)), this, SLOT(toggleBookmarkBar(bool)));
    showBookmarksAction->setCheckable(true);
    showBookmarksAction->setShortcut(QApplication::translate("Bookmarks", "Ctrl+b", 0));
    bool bookmarks = setting("show_bookmarks", QVariant(true)).toBool();
    showBookmarksAction->setChecked(bookmarks);
    toggleBookmarkBar(bookmarks);
    
    extensionMenu()->addSeparator();
    
    QAction* stopRecitationOnDeactivation = extensionMenu()->addAction("Stop Recitation On Deactivation");
    stopRecitationOnDeactivation->setObjectName("stopOnDeactivateAction");
    QObject::connect(stopRecitationOnDeactivation, SIGNAL(toggled(bool)), this, SLOT(toggleStopRecitationOnDeactivation(bool)));
    stopRecitationOnDeactivation->setCheckable(true);
    bool valStopRecitationOnDeactivation = setting("stop_recitation_on_deactivation", QVariant(true)).toBool();
    stopRecitationOnDeactivation->setChecked(valStopRecitationOnDeactivation);
    toggleStopRecitationOnDeactivation(valStopRecitationOnDeactivation);
    
}

void AlQuran::onContainerGeometryChanged(int w, int h)
{
    if (m_reciter == nullptr || m_splitter == nullptr) {
        return;
    }
    const int kCenterX = (w / 2) - (m_reciter->width() / 2);
    const int kBottom = h - m_reciter->height();
    m_reciter->move(kCenterX, kBottom);
    m_splitter->resize(w, h - m_reciter->height());
}

void AlQuran::onActivated()
{
    _TRACE;
}

void AlQuran::onDeactivated()
{
    _TRACE;
    // Stop if reciting
    if (m_stopRecitationOnDeactivation && m_reciter != nullptr) {
        m_reciter->stopIfPlaying();
    }
}

void AlQuran::onChapterChangedReciter(const quran::Chapter* chapter)
{
    _TRACE;
    if (m_reader != nullptr) {
        m_reader->changeChapter(chapter->name());
    }
}

void AlQuran::onVerseRangeChangedReciter(int from, int to)
{
    _TRACE;
    if (m_reader != nullptr) {
        m_reader->changeVerseRange(from, to);
    }
}

void AlQuran::onChapterChangedReader(const quran::Chapter* chapter)
{
    _TRACE;
    if (m_reciter != nullptr) {
        m_reciter->changeChapter(chapter->name());
    }
}

void AlQuran::onVerseRangeChangedReader(int from, int to)
{
    _TRACE;
    if (m_reciter != nullptr) {
        m_reciter->changeVerseRange(from, to);
    }
}

void AlQuran::onSelectedVerseChangedReciter(int verseNumber)
{
    _TRACE;
    if (m_reader != nullptr) {
        if (m_reader->currentVerseNumber() != verseNumber) {
            m_reader->highlightVerse(verseNumber);
        }
        m_reader->refreshJumpToText();
    }
}

void AlQuran::onSelectedVerseChangedReader(int index)
{
    _TRACE;
    if (m_reciter != nullptr) {
        m_reciter->changeVerse(index);
    }
}

void AlQuran::onBookmarkChanged(Bookmark* bookmark)
{
    _TRACE;
    if (bookmark == nullptr) {
        return;
    }
    m_reader->changeChapter(static_cast<quran::Chapter::Name>(bookmark->chapter()));
    m_reader->changeVerseRange(bookmark->verseFrom(), bookmark->verseTo());
    m_reader->highlightVerse(bookmark->selected());
}

void AlQuran::onJumpToTextChanged(const QString& txt)
{
    _TRACE;
    if (m_bookmarkBar != nullptr) {
        m_bookmarkBar->setCurrentJumpText(txt);
    }
}

void AlQuran::toggleReciter(bool val)
{
    _TRACE;
    m_reciter->setVisible(val);
    saveSetting("show_reciter", QVariant(val));
}

void AlQuran::toggleReader(bool val)
{
    _TRACE;
    m_reader->setVisible(val);
    saveSetting("show_reader", QVariant(val));
    if (!val) {
        m_reciter->showChapterSelector();
        m_reciter->showVerseRangeSelector();
        m_reciter->showCurrentVerseSelector();
    } else {
        m_reciter->hideChapterSelector();
        m_reciter->hideVerseRangeSelector();
        m_reciter->hideCurrentVerseSelector();
    }
    onContainerGeometryChanged(container()->width(), container()->height());
}

void AlQuran::toggleShowDuration(bool val)
{
    _TRACE;
    if (val) {
        m_reciter->showDurationWhenAvailable();
    } else {
        m_reciter->hideDurationWhenAvailable();
    }
    saveSetting("load_recite_duration", QVariant(val));
}

void AlQuran::toggleBookmarkBar(bool val)
{
    _TRACE;
    m_bookmarkBar->setVisible(val);
    saveSetting("show_bookmarks", QVariant(val));
    rightBarVisibilityToggle();
    onContainerGeometryChanged(extension()->containerWidth(), extension()->containerHeight());
}

void AlQuran::toggleStopRecitationOnDeactivation(bool val)
{
    _TRACE;
    m_stopRecitationOnDeactivation = val;
    saveSetting("stop_recitation_on_deactivation", QVariant(val));
}

void AlQuran::onToggledTranslation(bool val)
{
    _TRACE;
    saveSetting("load_translation", QVariant(val));
}

void AlQuran::onToggledTransliteration(bool val)
{
    _TRACE;
    saveSetting("load_transliteration", QVariant(val));
}

void AlQuran::onToggledTafseer(bool val)
{
    _TRACE;
    saveSetting("load_tafseer", QVariant(val));
}

void AlQuran::rightBarVisibilityToggle()
{
    _TRACE;
    // FIXME: This does not work in linux!
    //bool show = m_bookmarkBar->isVisible() /* || .. || */;
    //m_rightBar->setVisible(show);
}
