#ifndef MEMORIZATION_H
#define MEMORIZATION_H

#include "memorization_global.h"
#include "core/extension/abstract_extension.h"

class QuranReciter;
class QuranReader;

namespace quran {
class Chapter;
}

class Memorization : public AbstractExtension
{
    Q_OBJECT
    Q_INTERFACES(AbstractExtension)
    Q_PLUGIN_METADATA(IID "ProjectIslam.Extension.Memorization.v1.0")
private:
    static const unsigned int kMajorVersion = 1;
    static const unsigned int kMinorVersion = 0;
    static const char* kAuthor;
    static const char* kName;
    static const char* kTitle;
    static const char* kDescription;
public:
    explicit Memorization();
    virtual ~Memorization();
    
    virtual bool initialize(const el::Configurations* confFromLoader);
    
public slots:
    void onChapterChangedReciter(const quran::Chapter* chapter);
    void onVerseRangeChangedReciter(int, int);
    void onChapterChangedReader(const quran::Chapter* chapter);
    void onVerseRangeChangedReader(int, int);
    void onSelectedVerseChangedReciter(int);
    void onSelectedVerseChangedReader(int);
    void onContainerGeometryChanged();
private:
    QuranReciter* m_reciter;
    QuranReader* m_reader;
};

#endif // MEMORIZATION_H
