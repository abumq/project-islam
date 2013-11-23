#ifndef MEMORIZATION_H
#define MEMORIZATION_H

#include "core/extension/extension.h"
#include <QList>

class QuranReciter;
class QuranReader;

class QTableView;
class QStandardItemModel;

class Memorization : public Extension
{
public:
    Memorization(QWidget *parent, data::DataHolder* dataHolder);
    virtual ~Memorization();
    virtual void initialize(void);
protected:
    void resizeEvent(QResizeEvent *);
public slots:
    void onChapterChangedReciter(const quran::Chapter* chapter);
    void onVerseRangeChangedReciter(int, int);
    void onChapterChangedReader(const quran::Chapter* chapter);
    void onVerseRangeChangedReader(int, int);
private:
    QuranReciter* m_reciter;
    QuranReader* m_reader;
    QTableView* m_tableView;
    QStandardItemModel* m_model;
};

#endif // MEMORIZATION_H
