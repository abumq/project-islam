#ifndef QURAN_HH
#define QURAN_HH

#include <vector>
#include <string>
#include "quran/verse.hh"
#include "quran/chapter.hh"

namespace quran {
class Quran {
public:
    Quran(void);
    const quran::Verse* readVerse(Chapter::Name chapter, int verseNumber) const;
    const quran::Chapters& chapters(void) const;
    const quran::Chapter* getChapter(int i) const;
    bool ready(void) const;
private:
    void load(void);
    quran::Chapters m_chapters;
    bool m_ready;
};
} // namespace quran
#endif // QURAN_HH
