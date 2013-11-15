#ifndef QURAN_READER_HH
#define QURAN_READER_HH

#include <QTextDocument>

namespace quran {
class Chapter;
namespace ui {
class QuranReader : public QTextDocument
{
    Q_OBJECT
public:
    static const QString kSajdahText;
    static const unsigned int kMinSize = 10;
    static const unsigned int kMaxSize = 12;
    
    QuranReader(QObject* parent = 0);
    void update(const Chapter *chapter);
    QString highlightVerse(int verseNumber);
    QString find(const QString& str);
    void sizeUp(unsigned int threshold = 1);
    void sizeDown(unsigned int threshold = 1);
private:
    unsigned int m_size;
    QString m_currentHtml;
    quran::Chapter* m_currentChapter;
    
    void changeSize(unsigned int size);
};
}
}

#endif // QURAN_READER_HH
