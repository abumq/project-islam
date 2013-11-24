#include "quran_view.h"
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include "core/quran/quran.h"
QuranView::QuranView(quran::Quran* quran, QWidget *parent) :
    QGraphicsView(new QGraphicsScene(parent), parent),
    m_quran(quran),
    m_currentChapter(nullptr),
    m_selectedVerseTextItem(nullptr),
    m_ok(false)
{
    _TRACE;
    const QRectF rect = QRectF(0, 0, width(), height() * 100);
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
    
    // TODO: Set pos according to m_quran->readingDirection()
    int locH = 0;
    int locW = m_quran->readingDirection() == quran::Quran::ReadingDirection::LeftToRight ? 
        -(width() / 2) + 60 :
        (width() / 2) - 60;
        
    float size = 0;
    for (int i = from; i <= to; ++i) {
        const quran::Verse* verse = &m_currentChapter->verses().at(i);
        VerseTextItem* verseTextItem = new VerseTextItem(QString::fromStdWString(verse->text()), 
                                                         const_cast<quran::Verse*>(verse), nullptr);
        scene()->addItem(verseTextItem);
        verseTextItem->setFlag(QGraphicsItem::ItemIsMovable);
        verseTextItem->setPos(locW, locH);
        locH += 30;
        m_verseTextItems.insert(i, verseTextItem);
        if (i == from) {
            m_selectedVerseTextItem = verseTextItem;
            verseTextItem->highlight();
            size = verseTextItem->size();
            locW = verseTextItem->pos().x();
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

quran::Verse*QuranView::selectedVerse()
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
    // We dont ignore (newSize == m_fontSize) case since we
    // still want to space out verses
    if (m_verseTextItems.empty()) {
        return;
    }
    VerseTextItem* prev = nullptr;
    VerseTextItem* curr = nullptr;
    for (int key : m_verseTextItems.keys()) {
        prev = curr;
        curr = m_verseTextItems.value(key);
        curr->changeSize(newSize);
        if (prev != nullptr) {
            curr->setPos(curr->pos().x(), prev->size() + prev->pos().y() + (newSize * 3));
        }
    }
    m_fontSize = newSize;
}

float QuranView::fontSize() const
{
    return m_fontSize;
}

VerseTextItem::VerseTextItem(const QString& text, quran::Verse* verse, QGraphicsItem* parent) :
    QGraphicsTextItem(text, parent),
    m_plainText(text),
    m_verse(verse),
    m_highlighted(false)
{
    m_size = 10.0f;
    unhighlight();
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

void VerseTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsTextItem::mouseMoveEvent(event);
    setHtml(QString::number(pos().x()));
}
