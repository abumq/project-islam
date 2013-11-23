#include "core/quran/chapter.h"

namespace quran {

Chapter::Chapter(Classification classification, Name name, std::wstring&& arabicScriptName, 
                 std::wstring&& arabicName, std::wstring&& englishName, int rukuhCount, int versesCount, 
                 int sajdahCount, std::vector<int>&& positions, std::wstring&& muqattaat) : 
    m_classification(classification), m_name(name), m_arabicScriptName(std::move(arabicScriptName)), 
    m_arabicName(std::move(arabicName)), m_englishName(std::move(englishName)), m_rukuhCount(rukuhCount), 
    m_versesCount(versesCount), m_sajdahCount(sajdahCount), m_positions(std::move(positions)), m_muqattaat(std::move(muqattaat)) 
{
    
}

void Chapter::log(el::base::type::ostream_t &os) const 
{
    os << m_arabicName;
}

Chapter::Name Chapter::name(void) const
{ 
    return m_name; 
}

Chapter::Classification Chapter::classification(void) const 
{ 
    return m_classification; 
}

const std::wstring& Chapter::arabicScriptName(void) const
{ 
    return m_arabicScriptName; 
}

const std::wstring& Chapter::arabicName(void) const 
{ 
    return m_arabicName; 
}

const std::wstring& Chapter::englishName(void) const 
{
    return m_englishName; 
}

const std::wstring& Chapter::pronounciation(void) const 
{ 
    return m_pronounciation; 
}

int Chapter::rukuhCount(void) const 
{ 
    return m_rukuhCount; 
}

int Chapter::versesCount(void) const 
{ 
    return m_versesCount;
}

int Chapter::sajdahCount(void) const 
{ 
    return m_sajdahCount; 
}

const std::vector<int>& Chapter::positions(void) const 
{ 
    return m_positions; 
}

const std::wstring& Chapter::muqattaat(void) const  
{ 
    return m_muqattaat; 
}

const Verses& Chapter::verses(void) const 
{ 
    return m_verses; 
}

const Verse *Chapter::verse(int number) const
{
    Verses::const_iterator itVerse = m_verses.find(number);
    if (itVerse != m_verses.end()) {
        return &itVerse->second;
    }
}

} // namespace quran
