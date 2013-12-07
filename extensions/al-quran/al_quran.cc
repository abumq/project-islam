#include "al_quran.h"

#include <QLabel>
#include <QAction>

#include "core/controls/quran_reciter.h"
#include "core/controls/quran_reader.h"

_INITIALIZE_EASYLOGGINGPP

const char* AlQuran::kAuthor       = "Project Islam Authors";
const char* AlQuran::kName         = "Al-Quran";
const char* AlQuran::kTitle        = "Al-Qur'an";
const char* AlQuran::kDescription  = "Al-Qur'an recitation, reading and help memorize yourself.";

AlQuran::AlQuran() :
    m_reciter(nullptr),
    m_reader(nullptr)
{
    setExtensionInfo(ExtensionInfo(kMajorVersion, kMinorVersion, QString(kAuthor), 
                                        QString(kName), QString(kTitle), QString(kDescription)));
}

AlQuran::~AlQuran()
{
}

bool AlQuran::initialize()
{
    if (!ExtensionBase::initialize()) {
        // Do not proceed
        return false;
    }

    // Do not trace location before calling parent's initialize
    _TRACE;
    memory::deleteAll(m_reciter, m_reader);
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
    QObject::connect(m_reciter, SIGNAL(currentVerseChanged(int)), this, SLOT(onSelectedVerseChangedReciter(int)));
    
    initializeMenu();
    
    // Force trigger this slot
    onContainerGeometryChanged(extension()->container()->width(), extension()->container()->height());
    return true;
}

void AlQuran::initializeMenu()
{
    QAction* showReaderAction = extensionMenu()->addAction("Show Reader");
    showReaderAction->setCheckable(true);
    showReaderAction->setChecked(true);
    QObject::connect(showReaderAction, SIGNAL(toggled(bool)), m_reader, SLOT(setVisible(bool)));
    
    QAction* showReciterAction = extensionMenu()->addAction("Show Reciter");
    showReciterAction->setCheckable(true);
    showReciterAction->setChecked(true);
    QObject::connect(showReciterAction, SIGNAL(toggled(bool)), m_reciter, SLOT(setVisible(bool)));
}

void AlQuran::onContainerGeometryChanged(int w, int h)
{
    if (m_reciter != nullptr) {
        int centerW = (w / 2) - (m_reciter->width() / 2);
        int bottom = h - m_reciter->height();
        m_reciter->move(centerW, bottom);
    }
    m_reader->resize(w, h - m_reciter->height());
}

void AlQuran::onChapterChangedReciter(const quran::Chapter* chapter)
{
    if (m_reader != nullptr) {
        m_reader->changeChapter(chapter->name());
    }
}

void AlQuran::onVerseRangeChangedReciter(int from, int to)
{
    if (m_reader != nullptr) {
        m_reader->changeVerseRange(from, to);
    }
}

void AlQuran::onChapterChangedReader(const quran::Chapter* chapter)
{
    if (m_reciter != nullptr) {
        m_reciter->changeChapter(chapter->name());
    }
}

void AlQuran::onVerseRangeChangedReader(int from, int to)
{
    if (m_reciter != nullptr) {
        m_reciter->changeVerseRange(from, to);
    }
}

void AlQuran::onSelectedVerseChangedReciter(int verseNumber)
{
    if (m_reader != nullptr) {
        if (m_reader->currentVerseNumber() != verseNumber) {
            m_reader->highlightVerse(verseNumber);
        }
    }
}

void AlQuran::onSelectedVerseChangedReader(int index)
{
    if (m_reciter != nullptr) {
        m_reciter->changeVerse(index);
    }
}
