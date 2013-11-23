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
    VerseTextItem(const QString &text, QGraphicsItem* parent);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *o, QWidget *w);
    void highlight();
    void unhighlight();
private:
    QString m_plainText;
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
