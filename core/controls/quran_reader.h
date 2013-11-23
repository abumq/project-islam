#ifndef QURAN_READER_H
#define QURAN_READER_H

#include <QWidget>

#include "core/quran/chapter.h"

namespace Ui {
class QuranReader;
}

namespace data {
class DataHolder;
}

class QuranReader : public QWidget
{
    Q_OBJECT
    
public:
    QuranReader(data::DataHolder* dataHolder, QWidget *parent = 0);
    ~QuranReader();
    void update(int from, int to);
    void update(quran::Chapter::Name name, int from, int to);
private slots:
    void on_cboChapter_currentIndexChanged(int index);
    
private:
    Ui::QuranReader *ui;
    data::DataHolder* m_dataHolder;
    quran::Chapter* m_currentChapter;
};

#endif // QURAN_READER_H
