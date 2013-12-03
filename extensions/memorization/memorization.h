#ifndef MEMORIZATION_H
#define MEMORIZATION_H

#include "memorization_global.h"
#include "core/extension/extension_base.h"

class QuranReciter;
class QuranReader;

namespace quran {
class Chapter;
}

class Memorization : public ExtensionBase
{
   EXTENSION("ProjectIslam.Api.ExtensionBase.Memorization.v1.0")
private:
    static const unsigned int kMajorVersion = 1;
    static const unsigned int kMinorVersion = 0;
    static const char* kAuthor;
    static const char* kName;
    static const char* kTitle;
    static const char* kDescription;
public:
    Memorization();
    virtual ~Memorization();
    
    virtual bool initialize();
    
public slots:
    void onChapterChangedReciter(const quran::Chapter* chapter);
    void onVerseRangeChangedReciter(int, int);
    void onChapterChangedReader(const quran::Chapter* chapter);
    void onVerseRangeChangedReader(int, int);
    void onSelectedVerseChangedReciter(int);
    void onSelectedVerseChangedReader(int);
    void onContainerGeometryChanged(int, int);
private:
    QuranReciter* m_reciter;
    QuranReader* m_reader;
};

#endif // MEMORIZATION_H
