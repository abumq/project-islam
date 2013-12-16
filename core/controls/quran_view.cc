#include "quran_view.h"

#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QTextDocument>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QScrollBar>
#include <QResizeEvent>
#include <QMenu>

#include "core/utils/memory.h"
#include "core/logging/logging.h"
#include "core/quran/quran.h"

const float QuranView::kDefaultZoom = 1.0f;
const float QuranView::kDefaultZoomFactor = 1.1f;

QuranView::QuranView(quran::Quran* quran, quran::Quran* quranTranslation, 
                     quran::Quran* quranTransliteration, quran::Quran* quranTafseer, QWidget *parent) :
    QGraphicsView(new QGraphicsScene(parent), parent),
    m_ok(false),
    m_quran(quran),
    m_quranTranslation(quranTranslation),
    m_quranTransliteration(quranTransliteration),
    m_quranTafseer(quranTafseer),
    m_currentChapter(nullptr),
    m_selectedVerseTextItem(nullptr),
    m_showVerseNumbers(true),
    m_currFrom(1),
    m_currTo(1)
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

void QuranView::selectVerse(int verseNumber)
{
    _TRACE;
    if (m_verseTextItems.empty() || !m_verseTextItems.contains(verseNumber)) {
        return;
    }
    if (m_selectedVerseTextItem != nullptr) {
        // unhighlight selected verse
        m_selectedVerseTextItem->unhighlight();
        // unhighlight transliteration
        if (hasTransliteration()) {
            m_verseTextTransliterationItems.value(m_selectedVerseTextItem->verse()->number())->unhighlight();
        }
        // unhighlight translation
        if (hasTranslation()) {
            m_verseTextTranslationItems.value(m_selectedVerseTextItem->verse()->number())->unhighlight();
        }
        // unhighlight tafseer
        if (hasTafseer()) {
            m_verseTextTafseerItems.value(m_selectedVerseTextItem->verse()->number())->unhighlight();
        }
    }
    DVLOG(8) << "Selecting verse [" << verseNumber << "]";
    // Change currently selected verse and hightlight
    m_selectedVerseTextItem = m_verseTextItems.value(verseNumber);
    
    m_selectedVerseTextItem->highlight();
    // highlight transliteration
    if (hasTransliteration()) {
        m_verseTextTransliterationItems.value(verseNumber)->highlight();
    }
    // highlight translation
    if (hasTranslation()) {
        m_verseTextTranslationItems.value(verseNumber)->highlight();
    }
    // highlight tafseer
    if (hasTafseer()) {
        m_verseTextTafseerItems.value(verseNumber)->highlight();
    }
    // Only scroll vertical bar
    verticalScrollBar()->setValue(m_selectedVerseTextItem->pos().y() * (m_zoomValue / 100));
    emit currentVerseChanged(verseNumber);
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
    scene()->clear();
    
    // Either we highlight previously selected verse or if none is selected
    // we highlight 'from'
    int verseNumberToHighlight = m_selectedVerseTextItem != nullptr ? 
                m_selectedVerseTextItem->verse()->number() : from;
    // If previously selected verse is out of range we highlight 'from
    if (verseNumberToHighlight < from || verseNumberToHighlight > to) {
        verseNumberToHighlight = from;
    }
    m_verseTextItems.clear();
    m_verseTextTranslationItems.clear();
    m_verseTextTransliterationItems.clear();
    m_selectedVerseTextItem = nullptr;
    bool isChapterChanged = m_currentChapter != chapter;
    m_currentChapter = CHECK_NOTNULL(chapter);
    m_ok = m_currentChapter != nullptr;
    m_currFrom = from;
    m_currTo = to;
    if (from > m_currentChapter->versesCount()) {
        LOG(WARNING) << "Updating verseFrom from [" << from << "] to [" << m_currentChapter->versesCount() << "], invalid range";
        from = m_currentChapter->versesCount();
    }
    if (to > m_currentChapter->versesCount()) {
        LOG(WARNING) << "Updating verseTo from [" << to << "] to [" << m_currentChapter->versesCount() << "], invalid range";
        to = m_currentChapter->versesCount();
    }
    if (to < from) {
        LOG(WARNING) << "Updating verseTo from [" << to << "] to [" << from << "]";
        to = from;
    }
    DVLOG(7) << "Updating reader for chapter [" << m_currentChapter->arabicName() 
             << "] verses [" << from << " - " << to << "]";
    int locY = 0;
    
    quran::Chapter* transliteratedChapter = nullptr;
    if (hasTransliteration()) {
        transliteratedChapter = const_cast<quran::Chapter*>(m_quranTransliteration->chapter(m_currentChapter->name()));
    }
    quran::Chapter* translatedChapter = nullptr;
    if (hasTranslation()) {
        translatedChapter = const_cast<quran::Chapter*>(m_quranTranslation->chapter(m_currentChapter->name()));
    }
    quran::Chapter* tafseerChapter = nullptr;
    if (hasTafseer()) {
        tafseerChapter = const_cast<quran::Chapter*>(m_quranTafseer->chapter(m_currentChapter->name()));
    }
    for (int i = from; i <= to; ++i) {
        const quran::Verse* verse = &m_currentChapter->verses().at(i);
        quran::Verse* transliteratedVerse = nullptr;
        if (hasTransliteration()) {
            transliteratedVerse = const_cast<quran::Verse*>(&transliteratedChapter->verses().at(i));
        }
        quran::Verse* translatedVerse = nullptr;
        if (hasTranslation()) {
            translatedVerse = const_cast<quran::Verse*>(&translatedChapter->verses().at(i));
        }        
        quran::Verse* tafseerVerse = nullptr;
        if (hasTafseer()) {
            tafseerVerse = const_cast<quran::Verse*>(&tafseerChapter->verses().at(i));
        }
        QString verseText = QString(m_showVerseNumbers ? arabicNumber(verse->number()) + " -  " : "") + 
                QString::fromStdWString(verse->text());
        if (verse->sajdah() && !VerseTextItem::kSajdahChar.isEmpty()) {
            verseText.append(" <b>" + VerseTextItem::kSajdahChar + "</b>");
        }
        if (verse->rukuh() && !VerseTextItem::kRukuhChar.isEmpty()) {
            verseText.append(" <b>" + VerseTextItem::kRukuhChar + "</b>");
        }
        if (verse->hizbQuarter() && !VerseTextItem::kHizbChar.isEmpty()) {
            verseText.append(" <b>" + VerseTextItem::kHizbChar + "</b>");
        }
        if (verse->manzil() && !VerseTextItem::kManzilChar.isEmpty()) {
            verseText.append(" <b>" + VerseTextItem::kManzilChar + "</b>");
        }
        VerseTextItem* verseTextItem = new VerseTextItem(verseText, 
                                                         const_cast<quran::Verse*>(verse), nullptr);
        scene()->addItem(verseTextItem);
        verseTextItem->setY(locY);
        locY += kSpaceBetweenVerses;
        m_verseTextItems.insert(i, verseTextItem);
        // Transliteration
        if (hasTransliteration()) {
            VerseTextItem* transliteratedVerseTextItem = new VerseTextItem(QString::fromStdWString(transliteratedVerse->text()), 
                                                                           transliteratedVerse, nullptr);
            scene()->addItem(transliteratedVerseTextItem);
            transliteratedVerseTextItem->setY(locY);
            locY += kSpaceBetweenVerses;
            if (!hasTranslation() && !hasTafseer()) {
                locY += 10;
            }
            m_verseTextTransliterationItems.insert(i, transliteratedVerseTextItem);
        }
        // Translation
        if (hasTranslation()) {
            VerseTextItem* translatedVerseTextItem = new VerseTextItem(QString::fromStdWString(translatedVerse->text()), 
                                                                       translatedVerse, nullptr);
            scene()->addItem(translatedVerseTextItem);
            translatedVerseTextItem->setY(locY);
            locY += kSpaceBetweenVerses;
            if (!hasTafseer()) {
                locY += kSpaceBetweenVerses + 10;
            }
            m_verseTextTranslationItems.insert(i, translatedVerseTextItem);
        }
        // Tafseer
        if (hasTafseer()) {
            VerseTextItem* tafseerVerseTextItem = new VerseTextItem(QString::fromStdWString(tafseerVerse->text()), 
                                                                    tafseerVerse, nullptr);
            scene()->addItem(tafseerVerseTextItem);
            tafseerVerseTextItem->setY(locY);
            locY += kSpaceBetweenVerses + 10;
            m_verseTextTafseerItems.insert(i, tafseerVerseTextItem);
        }
        // Highlight verse
        if (i == verseNumberToHighlight) {
            // First verse i.e, val 'from'
            m_selectedVerseTextItem = verseTextItem;
            m_selectedVerseTextItem->highlight();
            
            if (hasTransliteration()) {
                m_verseTextTransliterationItems.value(i)->highlight();
            }
            if (hasTranslation()) {
                m_verseTextTranslationItems.value(i)->highlight();
            }
            if (hasTafseer()) {
                m_verseTextTafseerItems.value(i)->highlight();
            }
        }
    }
    updateView();
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

quran::Verse* QuranView::selectedVerse() const
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

void QuranView::updateView()
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
        // Check max width from transliterations
        if (!m_verseTextTransliterationItems.empty() && m_verseTextTransliterationItems.contains(key)) {
            curr = m_verseTextTransliterationItems.value(key);
            if (maxWidth < curr->textWidth()) {
                maxWidth = curr->textWidth();
            }
        }
        // Check max width from translation
        if (!m_verseTextTranslationItems.empty() && m_verseTextTranslationItems.contains(key)) {
            curr = m_verseTextTranslationItems.value(key);
            if (maxWidth < curr->textWidth()) {
                maxWidth = curr->textWidth();
            }
        }
        // Check max width from tafseer
        if (!m_verseTextTafseerItems.empty() && m_verseTextTafseerItems.contains(key)) {
            curr = m_verseTextTafseerItems.value(key);
            if (maxWidth < curr->textWidth()) {
                maxWidth = curr->textWidth();
            }
        }
    }
    
    int h = m_verseTextItems.count() * kSpaceBetweenVerses + m_verseTextItems.count();
    
    int multi = 0;
    if (hasTransliteration() && hasTranslation() && hasTafseer()) {
        multi = 3;
    } else if (((hasTransliteration() && hasTranslation()) && !hasTafseer()) ||
               ((hasTransliteration() && hasTafseer()) && !hasTranslation()) ||
               ((hasTranslation() && hasTafseer()) && !hasTransliteration())||
               (hasTranslation() || hasTafseer() || hasTransliteration())) {
        multi = 2;
    }
    ++multi; // For original verse
    h *= multi;
    h += multi > 1 ? m_verseTextItems.count() * 10 : 0;
    
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

void QuranView::setShowVerseNumbers(bool showVerseNumbers)
{
    m_showVerseNumbers = showVerseNumbers;
}

bool QuranView::showVerseNumbers() const
{
    return m_showVerseNumbers;
}

void QuranView::turnOnTranslation(quran::Quran* translationQuran)
{
    _TRACE;
    m_quranTranslation = translationQuran;
    update(m_currFrom, m_currTo);
    emit translationToggled(true);
}

void QuranView::turnOffTranslation()
{
    _TRACE;
    m_quranTranslation = nullptr;
    QList<VerseTextItem*> list = m_verseTextTranslationItems.values();
    for (int i = list.size() - 1; i >= 0; --i) {
        delete list.at(i);
    }
    m_verseTextTranslationItems.clear();
    update(m_currFrom, m_currTo);
    emit translationToggled(false);
}

void QuranView::turnOffTransliteration()
{
    _TRACE;
    m_quranTransliteration = nullptr;
    QList<VerseTextItem*> list = m_verseTextTransliterationItems.values();
    for (int i = list.size() - 1; i >= 0; --i) {
        delete list.at(i);
    }
    m_verseTextTransliterationItems.clear();
    update(m_currFrom, m_currTo);
    emit transliterationOnToggled(false);
}

void QuranView::turnOnTransliteration(quran::Quran* transliterationQuran)
{
    _TRACE;
    m_quranTransliteration = transliterationQuran;
    update(m_currFrom, m_currTo);
    emit transliterationOnToggled(true);
}

void QuranView::turnOffTafseer()
{   
    _TRACE;
    m_quranTafseer = nullptr;
    QList<VerseTextItem*> list = m_verseTextTafseerItems.values();
    for (int i = list.size() - 1; i >= 0; --i) {
        delete list.at(i);
    }
    m_verseTextTafseerItems.clear();
    update(m_currFrom, m_currTo);
    emit tafseerToggled(false);
    
}

void QuranView::turnOnTafseer(quran::Quran* tafseerQuran)
{
    _TRACE;
    m_quranTafseer = tafseerQuran;
    update(m_currFrom, m_currTo);
    emit tafseerToggled(true);
}

bool QuranView::hasTranslation() const
{
    return m_quranTranslation != nullptr && m_quranTranslation->ready();
}

bool QuranView::hasTransliteration() const
{
    return m_quranTransliteration != nullptr && m_quranTransliteration->ready();
}

bool QuranView::hasTafseer() const
{
    return m_quranTafseer != nullptr && m_quranTafseer->ready();
}

void QuranView::jumpTo(const QString& jumpToText)
{
    _TRACE;
    int chapter = m_currentChapter == nullptr ? 1 : static_cast<int>(m_currentChapter->name());
    int verseFrom = m_currFrom;
    int verseTo = m_currTo;
    int selected = m_currFrom;
    bool ok = false;
    QStringList tokens1 = jumpToText.split(":");
    if (tokens1.size() >= 2) {
        // We expect at least format 
        // 1:1
        chapter = tokens1.at(0).toInt(&ok);
        if (ok) {
            ok = chapter >= 1 && chapter <= quran::Quran::kChapterCount;
            QStringList tokensVerses = tokens1.at(1).split("-");
            verseFrom = tokensVerses.at(0).toInt(&ok);
            if (ok && tokensVerses.size() >= 2) {
                verseTo = tokensVerses.at(1).toInt(&ok);
                if (ok && tokens1.size() >= 3) {
                    selected = tokens1.at(2).toInt(&ok);
                }
            }
        }
    }
    if (ok) {
        update(static_cast<quran::Chapter::Name>(chapter), verseFrom, verseTo);
        selectVerse(selected);
    } else {
        LOG(ERROR) << "Unable to parse [" << jumpToText << "] as Qur'anic verse";
    }
}

QString QuranView::jumpToText() const
{
    _TRACE;
    if (m_currentChapter == nullptr) {
        DLOG(INFO) << "Current chapter is null";
        return QString();
    }
    return QString::number(static_cast<int>(m_currentChapter->name())) + 
            ":" + QString::number(m_currFrom) + 
            (m_currFrom < m_currTo ? "-" + QString::number(m_currTo) : "") + 
            (m_selectedVerseTextItem != nullptr 
            && m_selectedVerseTextItem->verse()->number() > m_currFrom
            && m_selectedVerseTextItem->verse()->number() <= m_currTo
            ? (":" + QString::number(m_selectedVerseTextItem->verse()->number())) 
            : "");
}

QString QuranView::arabicNumber(int n)
{
    QString nstr = QString::number(n);
    nstr.replace("0", "۰");
    nstr.replace("1", "۱");
    nstr.replace("2", "۲");
    nstr.replace("3", "۳");
    nstr.replace("4", "٤");
    nstr.replace("5", "۵");
    nstr.replace("6", "٦");
    nstr.replace("7", "٧");
    nstr.replace("8", "٨");
    nstr.replace("9", "٩");
    return nstr;
}

const QString VerseTextItem::kSajdahChar = QString::fromUtf8("۩");
const QString VerseTextItem::kRukuhChar = QString::fromUtf8("(؏)");
const QString VerseTextItem::kHizbChar = QString::fromUtf8("");
const QString VerseTextItem::kManzilChar = QString::fromUtf8("");

VerseTextItem::VerseTextItem(const QString& text, quran::Verse* verse, QGraphicsItem* parent) :
    QGraphicsTextItem(text, parent),
    m_plainText(text),
    m_verse(verse),
    m_highlighted(false),
    m_alignment(Qt::AlignLeft)
{
    unhighlight();
    setTextWidth(boundingRect().width() * 1.1);
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
