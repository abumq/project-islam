#ifndef MEMORIZATION_H
#define MEMORIZATION_H

#include "extensions/memorization/memorization_global.h"
#include "core/extension/extension.h"

class QuranReciter;
class QuranReader;

namespace quran {
class Chapter;
}

class MEMORIZATION_EXPORT Memorization : public Extension
{
    Q_OBJECT

private:
    static const int kMajorVersion = 0;
    static const int kMinorVersion = 0;
    static const int kPatchVersion = 1;
public:
    Memorization(QWidget *parent, data::DataHolder* dataHolder);
    virtual ~Memorization();
    virtual void initialize(void);
    
    int majorVersion(void) const;
    int minorVersion(void) const;
    int patchVersion(void) const;
    
protected:
    void resizeEvent(QResizeEvent *);
public slots:
    void onChapterChangedReciter(const quran::Chapter* chapter);
    void onVerseRangeChangedReciter(int, int);
    void onChapterChangedReader(const quran::Chapter* chapter);
    void onVerseRangeChangedReader(int, int);
    void onSelectedVerseChangedReciter(int);
    void onSelectedVerseChangedReader(int);
private:
    QuranReciter* m_reciter;
    QuranReader* m_reader;
};

#endif // MEMORIZATION_H
