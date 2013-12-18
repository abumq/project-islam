#ifndef QURAN_VIEW_H
#define QURAN_VIEW_H

#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QHash>

#include "quran/chapter.h"

namespace quran {
class Quran;
}

class VerseTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    static const QString kSajdahChar;
    static const QString kRukuhChar;
    static const QString kHizbChar;
    static const QString kManzilChar;
    
    VerseTextItem(const QString &text = QString(), quran::Verse* verse = 0, QGraphicsItem* parent = 0);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *o, QWidget *w);

    void highlight();
    void unhighlight();
    
    quran::Verse* verse();
    
    void setAlignment(Qt::Alignment alignment);
private:
    QString m_plainText;
    quran::Verse* m_verse;
    bool m_highlighted;
    Qt::Alignment m_alignment;
};

class QuranView : public QGraphicsView
{
    Q_OBJECT
public:
    static const int kSpaceBetweenVerses = 30;
    
    static const float kDefaultZoom;
    static const float kDefaultZoomFactor;
    QuranView(quran::Quran* quran, quran::Quran* quranTranslation, 
    quran::Quran* quranTransliteration, quran::Quran* quranTafseer, QWidget* parent);    
    virtual ~QuranView();
    
    quran::Chapter* currentChapter() const;
    
    void selectVerse(int verseNumber);
    void update(int from, int to);
    void update(quran::Chapter* chapter, int from, int to);
    void update(quran::Chapter::Name name, int from, int to);
    quran::Verse* selectedVerse() const;
    bool ok() const;
    
    float zoomValue() const;
    void scaleToDefault();
    void zoomIn(float scaleFactor = kDefaultZoomFactor);
    void zoomOut(float scaleFactor = kDefaultZoomFactor);
    
    void updateView();
    
    void setShowVerseNumbers(bool showVerseNumbers);
    bool showVerseNumbers() const;
    
    void turnOffTranslation();
    void turnOnTranslation(quran::Quran* translationQuran);
    
    void turnOffTransliteration();
    void turnOnTransliteration(quran::Quran* transliterationQuran);
    
    void turnOffTafseer();
    void turnOnTafseer(quran::Quran* tafseerQuran);
    
    bool hasTranslation() const;
    bool hasTransliteration() const;
    bool hasTafseer() const;
    
    void jumpTo(const QString& jumpToText);
    QString jumpToText() const;
    
    static QString arabicNumber(int n);
signals:
    void chapterChanged(const quran::Chapter*);
    void verseRangeChanged(int, int);
    void currentVerseChanged(int);
    void translationToggled(bool);
    void transliterationOnToggled(bool);
    void tafseerToggled(bool);
private:
    bool m_ok;
    quran::Quran* m_quran;
    quran::Quran* m_quranTranslation;
    quran::Quran* m_quranTransliteration;
    quran::Quran* m_quranTafseer;
    quran::Chapter* m_currentChapter;
    QMap<int, VerseTextItem*> m_verseTextItems;
    QMap<int, VerseTextItem*> m_verseTextTranslationItems;
    QMap<int, VerseTextItem*> m_verseTextTransliterationItems;
    QMap<int, VerseTextItem*> m_verseTextTafseerItems;
    VerseTextItem* m_selectedVerseTextItem;
    bool m_showVerseNumbers;
    int m_currFrom;
    int m_currTo;
    float m_zoomValue;
    
    void updateView(float valW, float spaceBw);
};

#endif // QURAN_VIEW_H
