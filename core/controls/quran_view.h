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
public:
    VerseTextItem(const QString &text = QString(), quran::Verse* verse = 0, QGraphicsItem* parent = 0);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *o, QWidget *w);
    void highlight();
    void unhighlight();
    quran::Verse* verse();
private:
    QString m_plainText;
    quran::Verse* m_verse;
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
    
signals:
    void chapterChanged(const quran::Chapter*);
    void verseRangeChanged(int, int);
    void selectedVerseChanged(int);
public slots:
private:
    quran::Quran* m_quran;
    quran::Chapter* m_currentChapter;
    QHash<int, VerseTextItem*> m_versesHash;
    VerseTextItem* m_selectedVerseTextItem;
};

#endif // QURAN_VIEW_H
