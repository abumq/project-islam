#ifndef MEMORIZATION_H
#define MEMORIZATION_H

#include "memorization_global.h"
#include "core/extension/abstract_extension.h"

class QuranReciter;
class QuranReader;

namespace quran {
class Chapter;
}

class Memorization : public QObject, public AbstractExtension
{
    Q_OBJECT
    Q_INTERFACES(AbstractExtension)
    Q_PLUGIN_METADATA(IID "ProjectIslam.Extension.Memorization.v1.0")
private:
    static const int kMajorVersion = 1;
    static const int kMinorVersion = 0;
    static const int kPatchVersion = 0;
    static const char* kAuthor;
    static const char* kName;
    static const char* kTitle;
    static const char* kDescription;
public:
    explicit Memorization(QObject *parent = 0);
    virtual ~Memorization();
    
    virtual bool initialize(bool initFromLoader, const el::Configurations* confFromLoader);
    
    // Extension Info
    virtual int majorVersion(void) const;
    virtual int minorVersion(void) const;
    virtual int patchVersion(void) const;
    virtual const char* author(void) const;
    virtual QString name() const;
    virtual QString title() const;
    virtual QString description() const;
    
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
