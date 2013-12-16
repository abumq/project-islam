#ifndef VERSE_HH
#define VERSE_HH

#include <vector>
#include <map>
#include <string>
#include <ostream>

namespace quran {
class Quran;
class Chapter;
class Verse {
public:
    Verse(int id, int number, std::wstring&& text, Chapter* chapter, 
          bool rukuh = false, bool sajdah = false, bool manzil = false, bool hizbQuarter = false);
    
    std::wostream& operator<<(std::wostream& os) const;
    
    int id(void) const;
    int number(void) const;
    bool rukuh(void) const;
    bool sajdah(void) const;
    bool manzil(void) const;
    bool hizbQuarter(void) const;
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
    bool m_manzil;
    bool m_hizbQuarter;
};
typedef std::pair<int, quran::Verse> SingleVerse;
typedef std::map<int, quran::Verse> Verses;
} // namespace quran
#endif // VERSE_HH
