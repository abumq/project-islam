#ifndef QURAN_VIEW_H
#define QURAN_VIEW_H

#include <QGraphicsView>
#include <QGraphicsTextItem>

#include "core/quran/chapter.h"
#include <QHash>
namespace quran {
class Quran;
}

class VerseTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    static const float kDefaultFontSize;
    VerseTextItem(const QString &text = QString(), quran::Verse* verse = 0, QGraphicsItem* parent = 0);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *o, QWidget *w);
    
    void highlight();
    void unhighlight();
    
    quran::Verse* verse();
    
    void sizeUp(float threshold);
    void sizeDown(float threshold);
    void changeSize(float newSize);
    float size() const;
    
    void setAlignment(Qt::Alignment alignment);
private:
    QString m_plainText;
    quran::Verse* m_verse;
    bool m_highlighted;
    float m_size;
    Qt::Alignment m_alignment;
};

class QuranView : public QGraphicsView
{
    Q_OBJECT
public:
    QuranView(quran::Quran* quran, QWidget* parent);    
    virtual ~QuranView();
    
    quran::Chapter* currentChapter() const;
    
    void selectVerse(int verse);
    void update(int from, int to);
    void update(quran::Chapter* chapter, int from, int to);
    void update(quran::Chapter::Name name, int from, int to);
    quran::Verse* selectedVerse();
    bool ok() const;
    void sizeUp(float threshold);
    void sizeDown(float threshold);
    void changeSize(float newSize);
    float fontSize() const;
signals:
    void chapterChanged(const quran::Chapter*);
    void verseRangeChanged(int, int);
    void currentVerseChanged(int);
private:
    quran::Quran* m_quran;
    quran::Chapter* m_currentChapter;
    QMap<int, VerseTextItem*> m_verseTextItems;
    VerseTextItem* m_selectedVerseTextItem;
    bool m_ok;
    float m_fontSize;
    
    void updateView(float valW, float spaceBw);
};

#endif // QURAN_VIEW_H
