#include "memorization.h"

#include "core/controls/quran_reciter.h"
#include "core/controls/quran_reader.h"

_INITIALIZE_EASYLOGGINGPP

const char* Memorization::kAuthor       = "Project Islam Authors";
const char* Memorization::kName         = "Memorization";
const char* Memorization::kTitle        = "Quran Memorization";
const char* Memorization::kDescription  = "Memorize glorious Quran with help of this extension. " \
                                            "This extension helps you keep track of your progress " \
                                            "of Quran memorization.";

Memorization::Memorization() :
    m_reciter(nullptr),
    m_reader(nullptr)
{
    setExtensionInfo(ExtensionInfo(kMajorVersion, kMinorVersion, QString(kAuthor), 
                                        QString(kName), QString(kTitle), QString(kDescription)));
}

Memorization::~Memorization()
{
}

bool Memorization::initialize(const el::Configurations* confFromLoader)
{
    if (!AbstractExtension::initialize(confFromLoader)) {
        // Do not proceed
        return false;
    }

    // Do not trace location before calling parent's initialize
    _TRACE;

    if (m_reciter != nullptr) {
        delete m_reciter;
        m_reciter = nullptr;
    }
    if (m_reader != nullptr) {
        delete m_reader;
        m_reader = nullptr;
    }
    m_reciter = new QuranReciter(extension()->dataHolder()->quranArabic(), extension()->container());
    m_reader = new QuranReader(extension()->dataHolder()->quranArabic(), extension()->container());
    m_reciter->hideChapterSelector();
    m_reciter->hideVerseRangeSelector();
    m_reciter->hideCurrentVerseSelector();
    QObject::connect(m_reciter, SIGNAL(chapterChanged(const quran::Chapter*)), this, SLOT(onChapterChangedReciter(const quran::Chapter*)));
    QObject::connect(m_reciter, SIGNAL(verseRangeChanged(int,int)), this, SLOT(onVerseRangeChangedReciter(int,int)));
    QObject::connect(m_reader, SIGNAL(chapterChanged(const quran::Chapter*)), this, SLOT(onChapterChangedReader(const quran::Chapter*)));
    QObject::connect(m_reader, SIGNAL(verseRangeChanged(int,int)), this, SLOT(onVerseRangeChangedReader(int,int)));
    QObject::connect(m_reader, SIGNAL(currentVerseChanged(int)), this, SLOT(onSelectedVerseChangedReader(int)));
    QObject::connect(m_reciter, SIGNAL(currentVerseChanged(int)), this, SLOT(onSelectedVerseChangedReciter(int)));
    
    // Force trigger this slot
    onContainerGeometryChanged();
    return true;
}

void Memorization::onContainerGeometryChanged()
{
    if (m_reciter != nullptr) {
        int centerW = (extension()->container()->width() / 2) - (m_reciter->width() / 2);
        int bottom = extension()->container()->height() - m_reciter->height() - 100;
        m_reciter->move(centerW, bottom);
    }
}

void Memorization::onChapterChangedReciter(const quran::Chapter* chapter)
{
    if (m_reader != nullptr) {
        m_reader->changeChapter(chapter->name());
    }
}

void Memorization::onVerseRangeChangedReciter(int from, int to)
{
    if (m_reader != nullptr) {
        m_reader->changeVerseRange(from, to);
    }
}

void Memorization::onChapterChangedReader(const quran::Chapter* chapter)
{
    if (m_reciter != nullptr) {
        m_reciter->changeChapter(chapter->name());
    }
}

void Memorization::onVerseRangeChangedReader(int from, int to)
{
    if (m_reciter != nullptr) {
        m_reciter->changeVerseRange(from, to);
    }
}

void Memorization::onSelectedVerseChangedReciter(int verseNumber)
{
    if (m_reader != nullptr) {
        if (m_reader->currentVerseNumber() != verseNumber) {
            m_reader->highlightVerse(verseNumber);
        }
    }
}

void Memorization::onSelectedVerseChangedReader(int index)
{
    if (m_reciter != nullptr) {
        m_reciter->changeVerse(index);
    }
}
