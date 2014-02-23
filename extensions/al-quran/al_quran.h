#ifndef AL_QURAN_H
#define AL_QURAN_H

#include "core/extension/extension_base.h"

class QuranReciter;
class QuranReader;
class Bookmark;
class BookmarksBar;
class QSplitter;

namespace quran {
class Chapter;
}

class AlQuran : public ExtensionBase
{
    Q_OBJECT
    Q_INTERFACES(ExtensionBase)
    Q_PLUGIN_METADATA(IID "ProjectIslam.Api.ExtensionBase.AlQuran.v0.1.0")
private:
    static const unsigned int kMajorVersion = 0;
    static const unsigned int kMinorVersion = 1;
    static const unsigned int kPatchVersion = 0;
public:
    AlQuran();
    ~AlQuran();
    
    virtual bool initialize(int argc, const char **argv);
    void initializeMenu();
    
    virtual inline unsigned int getMajorVersion() { return kMajorVersion; }
    virtual inline unsigned int getMinorVersion() { return kMinorVersion; }
    virtual inline unsigned int getPatchVersion() { return kPatchVersion; }
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
    void toggleShowDuration(bool);
    void toggleBookmarkBar(bool);
    void toggleStopRecitationOnDeactivation(bool);
    
    void onToggledTranslation(bool);
    void onToggledTransliteration(bool);
    void onToggledTafseer(bool);
private:
    QuranReciter* m_reciter;
    QuranReader* m_reader;
    QSplitter* m_splitter;
    QWidget* m_rightBar;
    BookmarksBar* m_bookmarkBar;
    bool m_stopRecitationOnDeactivation;
    
    void rightBarVisibilityToggle();
};

#endif // AL_QURAN_H
