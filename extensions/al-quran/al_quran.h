#ifndef AL_QURAN_H
#define AL_QURAN_H

#include "core/extension/extension_base.h"

class QuranReciter;
class QuranReader;
class Bookmark;
class QDockWidget;

namespace quran {
class Chapter;
}

class AlQuran : public ExtensionBase
{
    Q_OBJECT
    Q_INTERFACES(ExtensionBase)
    Q_PLUGIN_METADATA(IID "ProjectIslam.Api.ExtensionBase.AlQuran.v1.0")
private:
    static const unsigned int kMajorVersion = 1;
    static const unsigned int kMinorVersion = 0;
    static const unsigned int kPatchVersion = 0;
    static const char* kAuthor;
    static const char* kName;
    static const char* kTitle;
    static const char* kDescription;
public:
    AlQuran();
    ~AlQuran();
    
    virtual bool initialize(int argc, const char **argv);
    void initializeMenu();
    
public slots:

    // From interface
    virtual void onContainerGeometryChanged(int, int);
    virtual void onActivated();
    virtual void onDeactivated();
    
    void onChapterChangedReciter(const quran::Chapter* chapter);
    void onVerseRangeChangedReciter(int, int);
    void onChapterChangedReader(const quran::Chapter* chapter);
    void onVerseRangeChangedReader(int, int);
    void onSelectedVerseChangedReciter(int);
    void onSelectedVerseChangedReader(int);
    void onBookmarkChanged(Bookmark* bookmark);
    void onJumpToTextChanged(const QString&);
    
    void toggleReciter(bool);
    void toggleReader(bool);
    void toggleBookmarkBar(bool);
    
    void onToggledTranslation(bool);
    void onToggledTransliteration(bool);
    void onToggledTafseer(bool);
private:
    QuranReciter* m_reciter;
    QuranReader* m_reader;
    //BookmarksBar* m_bookmarkBar;
    QDockWidget* m_bookmarkBar;
};

#endif // AL_QURAN_H
