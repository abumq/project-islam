#include "quran_view.h"
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include "core/quran/quran.h"
QuranView::QuranView(quran::Quran* quran, QWidget *parent) :
    QGraphicsView(new QGraphicsScene(parent), parent),
    m_quran(quran),
    m_currentChapter(nullptr),
    m_selectedVerseTextItem(nullptr)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scene()->setSceneRect(0, 0, width(), height());
}

QuranView::~QuranView()
{
    scene()->clear();
    delete scene();
}

quran::Chapter* QuranView::currentChapter() const
{
    return m_currentChapter;
}

void QuranView::selectVerse(int verse)
{
    _TRACE;
    if (m_versesHash.empty() || !m_versesHash.contains(verse)) {
        return;
    }
    if (m_selectedVerseTextItem != nullptr) {
        // unhighlight selected verse
        m_selectedVerseTextItem->unhighlight();
    }
    LOG(INFO) << "Selecting verse [" << verse << "]";
    // Change currently selected verse and hightlight
    m_selectedVerseTextItem = m_versesHash.value(verse);
    
    m_selectedVerseTextItem->highlight();
    emit selectedVerseChanged(verse);
}

void QuranView::update(int from, int to)
{
    _TRACE;
    update(m_currentChapter, from, to);
}

void QuranView::update(quran::Chapter* chapter, int from, int to)
{
    _TRACE;
    TIMED_FUNC(timer);
    m_selectedVerseTextItem = nullptr;
    scene()->clear();
    m_versesHash.clear();
    bool isChapterChanged = m_currentChapter != chapter;
    m_currentChapter = CHECK_NOTNULL(chapter);
    CHECK(from <= m_currentChapter->versesCount()) << "Chapter: [" << m_currentChapter->arabicName() <<
                                                      "] only has [" << m_currentChapter->versesCount() << "] verses, requested verse [" << from << "]";
    CHECK(to <= m_currentChapter->versesCount()) << "Chapter: [" << m_currentChapter->arabicName() <<
                                                    "] only has [" << m_currentChapter->versesCount() << "] verses, requested verse [" << to << "]";
    CHECK(from <= to) << "Invalid range. Verses from [" << from << "] to [" << to << "] requested.";
    LOG(INFO) << "Updating reader for chapter [" << m_currentChapter->arabicName() 
              << "] verses [" << from << " - " << to << "]";
    int locH = -(height() / 2) + 20;
    int locW = -(width() / 2) + 60;
    for (int i = from; i <= to; ++i) {
        const quran::Verse* verse = &m_currentChapter->verses().at(i);
        VerseTextItem* verseTextItem = new VerseTextItem(QString::fromStdWString(verse->text()), const_cast<quran::Verse*>(verse), nullptr);
        scene()->addItem(verseTextItem);
        verseTextItem->setPos(locW, locH);
        locH += 20;
        m_versesHash.insert(i, verseTextItem);
        if (i == from) {
            m_selectedVerseTextItem = verseTextItem;
            verseTextItem->highlight();
        }
    }
    if (isChapterChanged) {
        emit chapterChanged(m_currentChapter);
    }
    emit verseRangeChanged(from, to);
}

void QuranView::update(quran::Chapter::Name chapter, int from, int to)
{
    _TRACE;
    update(const_cast<quran::Chapter*>(m_quran->chapter(chapter)), from, to);
}

quran::Verse*QuranView::selectedVerse()
{
    return m_selectedVerseTextItem == nullptr ? nullptr : m_selectedVerseTextItem->verse();
}

VerseTextItem::VerseTextItem(const QString& text, quran::Verse* verse, QGraphicsItem* parent) :
    QGraphicsTextItem(text, parent),
    m_plainText(text),
    m_verse(verse)
{
}

void VerseTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* o, QWidget* w)
{
    QGraphicsTextItem::paint(painter, o, w); 
}

void VerseTextItem::highlight()
{
    _TRACE;
    setHtml("<span style='background-color:#FFFF00;'>" + m_plainText + "</span>");
}

void VerseTextItem::unhighlight()
{
    _TRACE;
    setHtml(m_plainText);
}

quran::Verse* VerseTextItem::verse()
{
    return m_verse;
}
