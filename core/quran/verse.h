#ifndef VERSE_HH
#define VERSE_HH

#include <vector>
#include <map>
#include <string>
#include "core/logging.h"

namespace quran {
class Quran;
class Chapter;
class Verse : public el::Loggable {
public:
    Verse(int id, int number, std::wstring&& text, Chapter* chapter, bool rukuh = false, bool sajdah = false);
    void log(el::base::type::ostream_t& os) const;

    int id(void) const;
    int number(void) const;
    bool rukuh(void) const;
    bool sajdah(void) const;
    const std::wstring& text(void) const;
    const quran::Chapter* chapter(void) const;

private:
    friend class Quran;
    
    int m_id; // ID in database
    int m_number;
    std::wstring m_text;
    quran::Chapter* m_chapter;
    bool m_rukuh;
    bool m_sajdah;
};
typedef std::pair<int, quran::Verse> SingleVerse;
typedef std::map<int, quran::Verse> Verses;
} // namespace quran
#endif // VERSE_HH
