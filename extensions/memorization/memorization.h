#ifndef MEMORIZATION_H
#define MEMORIZATION_H

#include "extensions/memorization/memorization_global.h"
#include "core/extension/extension_interface.h"
#include "core/extension/extension.h"

class QuranReciter;
class QuranReader;

namespace quran {
class Chapter;
}

class Memorization : public QObject, public ExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(ExtensionInterface)
    Q_PLUGIN_METADATA(IID "ProjectIslam.Extension.Memorization.v1.0")
private:
    static const int kMajorVersion = 0;
    static const int kMinorVersion = 0;
    static const int kPatchVersion = 1;
public:
    explicit Memorization(QObject *parent = 0);
    virtual ~Memorization();
    
    void initialize(void);
    
    int majorVersion(void) const;
    int minorVersion(void) const;
    int patchVersion(void) const;
    
    Extension* extension();
public slots:
    void onChapterChangedReciter(const quran::Chapter* chapter);
    void onVerseRangeChangedReciter(int, int);
    void onChapterChangedReader(const quran::Chapter* chapter);
    void onVerseRangeChangedReader(int, int);
    void onSelectedVerseChangedReciter(int);
    void onSelectedVerseChangedReader(int);
    void updateView();
private:
    Extension* m_extension;
    QuranReciter* m_reciter;
    QuranReader* m_reader;
};

#endif // MEMORIZATION_H
