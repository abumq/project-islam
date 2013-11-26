#include "quran_view.h"
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QTextDocument>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QScrollBar>
#include "core/quran/quran.h"

const float QuranView::kDefaultZoom = 1.0f;
const float QuranView::kDefaultZoomFactor = 1.1f;

QuranView::QuranView(quran::Quran* quran, QWidget *parent) :
    QGraphicsView(new QGraphicsScene(parent), parent),
    m_quran(quran),
    m_currentChapter(nullptr),
    m_selectedVerseTextItem(nullptr),
    m_ok(false)
{
    _TRACE;
    const QRectF rect = QRectF(0, 0, 1, 1);
    scene()->setSceneRect(rect);
    scaleToDefault();
}

QuranView::~QuranView()
{
    _TRACE;
    scene()->clear();
    delete scene();
}

quran::Chapter* QuranView::currentChapter() const
{
    _TRACE;
    return m_currentChapter;
}

void QuranView::selectVerse(int verse)
{
    _TRACE;
    if (m_verseTextItems.empty() || !m_verseTextItems.contains(verse)) {
        return;
    }
    if (m_selectedVerseTextItem != nullptr) {
        // unhighlight selected verse
        m_selectedVerseTextItem->unhighlight();
    }
    DVLOG(8) << "Selecting verse [" << verse << "]";
    // Change currently selected verse and hightlight
    m_selectedVerseTextItem = m_verseTextItems.value(verse);
    
    m_selectedVerseTextItem->highlight();
    emit currentVerseChanged(verse);
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
    m_verseTextItems.clear();
    bool isChapterChanged = m_currentChapter != chapter;
    m_currentChapter = CHECK_NOTNULL(chapter);
    DCHECK(from <= m_currentChapter->versesCount()) << "Chapter: [" << m_currentChapter->arabicName() <<
                                                       "] only has [" << m_currentChapter->versesCount() << "] verses, requested verse [" << from << "]";
    DCHECK(to <= m_currentChapter->versesCount()) << "Chapter: [" << m_currentChapter->arabicName() <<
                                                     "] only has [" << m_currentChapter->versesCount() << "] verses, requested verse [" << to << "]";
    DCHECK(from <= to) << "Invalid range. Verses from [" << from << "] to [" << to << "] requested.";
    DVLOG(7) << "Updating reader for chapter [" << m_currentChapter->arabicName() 
             << "] verses [" << from << " - " << to << "]";
    m_ok = m_currentChapter != nullptr;
    
    int locY = 0;
    
    for (int i = from; i <= to; ++i) {
        const quran::Verse* verse = &m_currentChapter->verses().at(i);
        VerseTextItem* verseTextItem = new VerseTextItem(QString::fromStdWString(verse->text()), 
                                                         const_cast<quran::Verse*>(verse), nullptr);
        scene()->addItem(verseTextItem);
        verseTextItem->setY(locY);
        locY += 30;
        m_verseTextItems.insert(i, verseTextItem);
        if (i == from) {
            // First verse i.e, val 'from'
            m_selectedVerseTextItem = verseTextItem;
            verseTextItem->highlight();
        }
    }
    updateView(kDefaultZoom);
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

quran::Verse* QuranView::selectedVerse()
{
    _TRACE;
    return m_selectedVerseTextItem == nullptr ? nullptr : m_selectedVerseTextItem->verse();
}

bool QuranView::ok() const
{
    return m_ok;
}

float QuranView::zoomValue() const
{
    return m_zoomValue;
}

void QuranView::scaleToDefault()
{
    _TRACE;
    float v = kDefaultZoom;
    scale(v, v);
    m_zoomValue = 100.0f;
}

void QuranView::zoomIn(float scaleFactor)
{
    _TRACE;
    float v = scaleFactor;
    scale(v, v);
    m_zoomValue *= v;
}

void QuranView::zoomOut(float scaleFactor)
{
    _TRACE;
    float v = 1.0 / scaleFactor;
    scale(v, v);
    m_zoomValue *= v;
}

void QuranView::updateView(float newSize)
{
    _TRACE;
    scene()->setSceneRect(QRectF(0, 0, 1, 1)); // Will be updated in updateView()
    // We dont ignore (newSize == m_fontSize) case since we
    // still want to space out verses
    if (m_verseTextItems.empty()) {
        return;
    }
    int maxWidth = -2;
    for (int key : m_verseTextItems.keys()) {
        VerseTextItem* curr = m_verseTextItems.value(key);
        if (maxWidth < curr->textWidth()) {
            maxWidth = curr->textWidth();
        }
    }
    int h = m_verseTextItems.count() * 30 + (m_verseTextItems.count() * newSize);
    const QRectF rect = QRectF(0, 0, maxWidth, h);
    scene()->setSceneRect(rect);
    if (m_quran->readingDirection() == quran::Quran::ReadingDirection::LeftToRight) {
        horizontalScrollBar()->setValue(horizontalScrollBar()->minimum());
    } else {
        horizontalScrollBar()->setValue(horizontalScrollBar()->maximum());
    }
    // Regardless of quran->textDirection we will have to set it to left
    // since it's unicode's job to align it after we have set text width
    Qt::Alignment alignment = Qt::AlignLeft;
    VerseTextItem* curr = nullptr;
    for (int key : m_verseTextItems.keys()) {
        curr = m_verseTextItems.value(key);
        curr->setTextWidth(maxWidth);
        curr->setAlignment(alignment);
    }
}

VerseTextItem::VerseTextItem(const QString& text, quran::Verse* verse, QGraphicsItem* parent) :
    QGraphicsTextItem(text, parent),
    m_plainText(text),
    m_verse(verse),
    m_highlighted(false),
    m_alignment(Qt::AlignLeft)
{
    unhighlight();
    setTextWidth(boundingRect().width());
    setAlignment(m_alignment);
}

void VerseTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* o, QWidget* w)
{
    QGraphicsTextItem::paint(painter, o, w); 
}

void VerseTextItem::highlight()
{
    setHtml("<span style='background-color:#FFFF00;'>" + m_plainText + "</span>");
    m_highlighted = true;
}

void VerseTextItem::unhighlight()
{
    setHtml("<span>" + m_plainText + "</span>");
    m_highlighted = false;
}

quran::Verse* VerseTextItem::verse()
{
    return m_verse;
}

void VerseTextItem::setAlignment(Qt::Alignment alignment)
{
    m_alignment = alignment;
    
    QTextBlockFormat format;
    format.setAlignment(m_alignment);
    
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    
    setTextCursor(cursor); 
}
