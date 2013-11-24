#include "quran_view.h"
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QTextDocument>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QScrollBar>
#include "core/quran/quran.h"
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
    
    float size = 0;
    for (int i = from; i <= to; ++i) {
        const quran::Verse* verse = &m_currentChapter->verses().at(i);
        VerseTextItem* verseTextItem = new VerseTextItem(QString::fromStdWString(verse->text()), 
                                                         const_cast<quran::Verse*>(verse), nullptr);
        scene()->addItem(verseTextItem);
        verseTextItem->setY(locY);
        verseTextItem->setToolTip(QString::number(locY));
        locY += 30;
        m_verseTextItems.insert(i, verseTextItem);
        if (i == from) {
            // First verse i.e, val 'from'
            m_selectedVerseTextItem = verseTextItem;
            verseTextItem->highlight();
            if (m_fontSize == 0) {
                m_fontSize = VerseTextItem::kDefaultFontSize;
            }
            size = verseTextItem->size() == m_fontSize ? verseTextItem->size() : m_fontSize;
        }
    }
    changeSize(size);
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

void QuranView::sizeUp(float threshold)
{
    _TRACE;
    changeSize(m_fontSize + threshold);
}

void QuranView::sizeDown(float threshold)
{
    _TRACE;
    changeSize(m_fontSize - threshold);
}

void QuranView::changeSize(float newSize)
{
    _TRACE;
    scene()->setSceneRect(QRectF(0, 0, 1, 1)); // Will be updated in updateView()
    // We dont ignore (newSize == m_fontSize) case since we
    // still want to space out verses
    if (m_verseTextItems.empty()) {
        return;
    }
    int maxWidth = -2;
    int spaceBw = 30;
    VerseTextItem* prev = nullptr;
    VerseTextItem* curr = nullptr;
    for (int key : m_verseTextItems.keys()) {
        prev = curr;
        curr = m_verseTextItems.value(key);
        curr->changeSize(newSize);
        if (prev != nullptr) {
            curr->setPos(curr->pos().x(), prev->size() + prev->pos().y() + (newSize * 3));
            spaceBw = curr->pos().y() - prev->pos().y() - newSize;
            LOG(INFO) << spaceBw;
        }
        if (maxWidth < curr->textWidth()) {
            maxWidth = curr->textWidth();
        }
    }
    m_fontSize = newSize;
    updateView(maxWidth, spaceBw);
}

float QuranView::fontSize() const
{
    return m_fontSize;
}
void QuranView::updateView(float valW, float spaceBw)
{
    _TRACE;
    int h = m_verseTextItems.count() * spaceBw + (m_verseTextItems.count() * m_fontSize);
    const QRectF rect = QRectF(0, 0, valW, h);
    scene()->setSceneRect(rect);
    horizontalScrollBar()->setValue(horizontalScrollBar()->maximum());
    // Regardless of quran->textDirection we will have to set it to left
    // since it's unicode's job to align it after we have set text width
    Qt::Alignment alignment = Qt::AlignLeft;
    VerseTextItem* curr = nullptr;
    for (int key : m_verseTextItems.keys()) {
        curr = m_verseTextItems.value(key);
        curr->setTextWidth(valW);
        curr->setAlignment(alignment);
    }
}

const float VerseTextItem::kDefaultFontSize = 10.0f;

VerseTextItem::VerseTextItem(const QString& text, quran::Verse* verse, QGraphicsItem* parent) :
    QGraphicsTextItem(text, parent),
    m_plainText(text),
    m_verse(verse),
    m_highlighted(false),
    m_size(kDefaultFontSize),
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
    _TRACE;
    setHtml("<span style='font-size: " + QString::number(m_size) + "pt;background-color:#FFFF00;'>" + m_plainText + "</span>");
    m_highlighted = true;
}

void VerseTextItem::unhighlight()
{
    _TRACE;
    setHtml("<span style='font-size: " + QString::number(m_size) + "pt;'>" + m_plainText + "</span>");
    m_highlighted = false;
}

quran::Verse* VerseTextItem::verse()
{
    return m_verse;
}

void VerseTextItem::sizeUp(float threshold)
{
    _TRACE;
    changeSize(m_size + threshold);
}

void VerseTextItem::sizeDown(float threshold)
{
    _TRACE;
    changeSize(m_size - threshold);
}

void VerseTextItem::changeSize(float newSize)
{
    _TRACE;
    LOG(INFO) << "Changing size from [" << m_size << "] to [" << newSize << "]";
    m_size = newSize;
    if (m_highlighted) {
        highlight();
    } else {
        unhighlight();
    }
}

float VerseTextItem::size() const
{
    return m_size;
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
