#include "al_quran.h"

#include <QLabel>
#include <QAction>
#include <QDockWidget>

#include "bookmarks_bar.h"
#include "core/controls/quran_reciter.h"
#include "core/controls/quran_reader.h"

_INITIALIZE_EASYLOGGINGPP

const char* AlQuran::kAuthor       = "Project Islam Authors";
const char* AlQuran::kName         = "Al-Quran";
const char* AlQuran::kTitle        = "Al-Qur'an";
const char* AlQuran::kDescription  = "Al-Qur'an recitation, reading and help memorize yourself.";

AlQuran::AlQuran()
{
    memory::turnToNullPtr(m_reciter, m_reader, m_bookmarkBar);
    setExtensionInfo(ExtensionInfo(kMajorVersion, kMinorVersion, kPatchVersion, 
                                   QString(kAuthor), QString(kName), 
                                   QString(kTitle), QString(kDescription)));
    Q_INIT_RESOURCE(icons);
}

AlQuran::~AlQuran()
{
    // m_reader, m_reciter not available! deleted on ~extension()->container()
}

bool AlQuran::initialize(int argc, const char** argv)
{
    if (!ExtensionBase::initialize(argc, argv)) {
        // Do not proceed
        return false;
    }
    
    // Do not trace location before calling parent's initialize
    _TRACE;
    memory::deleteAll(m_reciter, m_reader, m_bookmarkBar);
    m_reciter = new QuranReciter(extension()->dataHolder()->quranArabic(), extension()->container());
    m_reader = new QuranReader(extension()->dataHolder()->quranArabic(), 
                               extension()->dataHolder()->quranTranslation(), extension()->dataHolder()->quranTransliteration(), 
                               extension()->container());    
    m_reciter->hideChapterSelector();
    m_reciter->hideVerseRangeSelector();
    m_reciter->hideCurrentVerseSelector();
    QObject::connect(m_reciter, SIGNAL(chapterChanged(const quran::Chapter*)), this, SLOT(onChapterChangedReciter(const quran::Chapter*)));
    QObject::connect(m_reciter, SIGNAL(verseRangeChanged(int,int)), this, SLOT(onVerseRangeChangedReciter(int,int)));
    QObject::connect(m_reader, SIGNAL(chapterChanged(const quran::Chapter*)), this, SLOT(onChapterChangedReader(const quran::Chapter*)));
    QObject::connect(m_reader, SIGNAL(verseRangeChanged(int,int)), this, SLOT(onVerseRangeChangedReader(int,int)));
    QObject::connect(m_reader, SIGNAL(currentVerseChanged(int)), this, SLOT(onSelectedVerseChangedReader(int)));
    QObject::connect(m_reader, SIGNAL(translationToggled(bool)), this, SLOT(onToggledTranslation(bool)));
    QObject::connect(m_reader, SIGNAL(transliterationOnToggled(bool)), this, SLOT(onToggledTransliteration(bool)));
    QObject::connect(m_reciter, SIGNAL(currentVerseChanged(int)), this, SLOT(onSelectedVerseChangedReciter(int)));
    QObject::connect(m_reader, SIGNAL(jumpToTextChanged(QString)), this, SLOT(onJumpToTextChanged(QString)));
    // Bookmarks bar
    m_bookmarkBar = new QDockWidget("Bookmarks", extension()->container());
    BookmarksBar* bar = new BookmarksBar(settingsKeyPrefix());
    m_bookmarkBar->setAllowedAreas(Qt::RightDockWidgetArea);
    m_bookmarkBar->setWidget(bar);
    m_bookmarkBar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    
    QObject::connect(bar, SIGNAL(selectionChanged(Bookmark*)), this, SLOT(onBookmarkChanged(Bookmark*)));
    
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
    // Force trigger this slot
    onContainerGeometryChanged(extension()->container()->width(), extension()->container()->height());
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
    
    QAction* showBookmarksAction = extensionMenu()->addAction("Bookmarks");
    QObject::connect(showBookmarksAction, SIGNAL(toggled(bool)), this, SLOT(toggleBookmarkBar(bool)));
    showBookmarksAction->setCheckable(true);
    bool bookmarks = setting("show_bookmarks", QVariant(true)).toBool();
    showBookmarksAction->setChecked(bookmarks);
    toggleBookmarkBar(bookmarks);
    
}

void AlQuran::onContainerGeometryChanged(int w, int h)
{
    const int kCenterX = (w / 2) - (m_reciter->width() / 2);
    const int kBottom = h - m_reciter->height();
    m_reciter->move(kCenterX, kBottom);
    
    if (m_bookmarkBar->isVisible()) {
        m_bookmarkBar->resize(BookmarksBar::kBookmarkBarWidth, h - m_reciter->height());
        m_bookmarkBar->move(w - BookmarksBar::kBookmarkBarWidth, 0);
    }
    
    m_reader->resize(w - (m_bookmarkBar->isVisible() ? BookmarksBar::kBookmarkBarWidth : 0), 
                     h - m_reciter->height());
}

void AlQuran::onActivated()
{
    _TRACE;
}

void AlQuran::onDeactivated()
{
    _TRACE;
    // Stop if reciting
    if (m_reciter != nullptr) {
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
        static_cast<BookmarksBar*>(m_bookmarkBar->widget())->setCurrentJumpText(txt);
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
}

void AlQuran::toggleBookmarkBar(bool val)
{
    _TRACE;
    m_bookmarkBar->setVisible(val);
    onContainerGeometryChanged(extension()->containerWidth(), extension()->containerHeight());
    saveSetting("show_bookmarks", QVariant(val));
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
