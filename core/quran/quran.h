#ifndef QURAN_HH
#define QURAN_HH

#include <vector>
#include <string>

#include "core/quran/verse.h"
#include "core/quran/chapter.h"

namespace quran {
class Quran {
public:
    static const int kChapterCount = 114;
    static const char* kQuranArabicDatabaseTable;
    static const char* kQuranChapterDatabaseTable;
    static const char* kQuranTransliterationDatabaseTable;
    static const char* kQuranTafseerDatabaseTable;
    static const char* kQuranDefaultTranslationDatabaseTable;
    static const char* kQuranDefaultTafseerDatabaseTable;
    
    enum class TextType {
        Original, Translation, Transliteration, Tafseer
    };
    enum class ReadingDirection {
        LeftToRight, RightToLeft
    };
    Quran(void);
    void load(const TextType& textType, const std::string& databaseTable);
    const quran::Verse* verse(Chapter::Name chapter, int verseNumber) const;
    const quran::Chapters& chapters(void) const;
    const quran::Chapter* chapter(Chapter::Name chapter) const;
    bool ready(void) const;
    TextType textType(void) const;
    ReadingDirection readingDirection(void) const;
private:
    quran::Chapters m_chapters;
    bool m_ready;
    TextType m_textType;
    ReadingDirection m_readingDirection;
};
} // namespace quran
#endif // QURAN_HH
