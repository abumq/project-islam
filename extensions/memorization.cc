#include "extensions/memorization.h"

#include <QResizeEvent>
#include <QTableView>
#include <QStandardItemModel>

#include "core/controls/quran_reciter.h"
#include "core/controls/quran_reader.h"

Memorization::Memorization(QWidget *parent, data::DataHolder* dataHolder) :
    Extension(parent, dataHolder, "Memorization", "Quran Memorization",
        "Memorize glorious Quran with help of this extension. " \
        "This extension helps you keep track of your progress " \
        "of Quran memorization."
    ),
    m_reciter(nullptr),
    m_reader(nullptr),
    m_tableView(nullptr)
{
    initialize();
}

Memorization::~Memorization()
{
    _TRACE;
    delete m_tableView;
    m_tableView = nullptr;
}

void Memorization::initialize()
{
    _TRACE;
    m_reciter = new QuranReciter(dataHolder()->quranArabic(), container());
    m_reader = new QuranReader(dataHolder()->quranArabic(), container());
    connect(m_reciter, SIGNAL(chapterChanged(const quran::Chapter*)), this, SLOT(onChapterChangedReciter(const quran::Chapter*)));
    connect(m_reciter, SIGNAL(verseRangeChanged(int,int)), this, SLOT(onVerseRangeChangedReciter(int,int)));
    connect(m_reader, SIGNAL(chapterChanged(const quran::Chapter*)), this, SLOT(onChapterChangedReader(const quran::Chapter*)));
    connect(m_reader, SIGNAL(verseRangeChanged(int,int)), this, SLOT(onVerseRangeChangedReader(int,int)));
    connect(m_reciter, SIGNAL(currentVerseChanged(int)), this, SLOT(onSelectedVerseChangedReciter(int)));
    m_tableView = new QTableView(container());
    m_tableView->hide();
}

void Memorization::resizeEvent(QResizeEvent* e)
{
    Extension::resizeEvent(e);
    if (m_reciter != nullptr) {
        int centerW = (container()->width() / 2) - (m_reciter->width() / 2);
        int bottom = container()->height() - m_reciter->height() - 100;
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
        if (m_reader->currentVerse() != verseNumber) {
            m_reader->highlightVerse(verseNumber);
        }
    }
}
