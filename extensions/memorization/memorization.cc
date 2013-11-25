#include "memorization.h"

#include <QResizeEvent>

#include "core/logging.h"
#include "core/controls/quran_reciter.h"
#include "core/controls/quran_reader.h"

_INITIALIZE_EASYLOGGINGPP

Memorization::Memorization(QObject *parent) :
    QObject(parent),
    m_extension(nullptr),
    m_reciter(nullptr),
    m_reader(nullptr)
{
    initialize();
}

Memorization::~Memorization()
{
    // No need to delete m_extension as plugin loader
    // i.e, ExtensionLoader deletes this anyway in fact doing this
    // will cause segmentation fault
}

Extension *Memorization::extension()
{
    if (m_extension == nullptr) {
        m_extension = new Extension(0, "Memorization", "Quran Memorization",
            "Memorize glorious Quran with help of this extension. " \
            "This extension helps you keep track of your progress " \
            "of Quran memorization."
        );
        QObject::connect(m_extension, SIGNAL(containerGeometryChanged()), this, SLOT(updateView()));
    }
    return m_extension;
}

void Memorization::initialize()
{
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
    
    updateView();
}

int Memorization::majorVersion() const
{
    return kMajorVersion;
}

int Memorization::minorVersion() const
{
    return kMinorVersion;
}

int Memorization::patchVersion() const
{
    return kPatchVersion;
}

void Memorization::updateView()
{
    if (m_reciter != nullptr) {
        int centerW = (extension()->container()->width() / 2) - (m_reciter->width() / 2);
        int bottom = extension()->container()->height() - m_reciter->height() - 100;
        LOG(INFO) << "Moving reciter to [" << center << "l; " << bottom << "b]";
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
