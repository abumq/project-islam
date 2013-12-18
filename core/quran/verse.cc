
#ifndef _LOGGER
#   define _LOGGER "quran"
#endif
#ifndef _PERFORMANCE_LOGGER
#   define _PERFORMANCE_LOGGER _LOGGER
#endif

#include "core/quran/verse.h"

namespace quran {

Verse::Verse(int id, int number, std::wstring&& text, Chapter* chapter, bool rukuh, bool sajdah, bool manzil, bool hizbQuarter) :
    m_id(id), m_number(number), m_text(std::move(text)), m_chapter(chapter), m_rukuh(rukuh), m_sajdah(sajdah),
    m_manzil(manzil), m_hizbQuarter(hizbQuarter)
{
}

std::wostream& Verse::operator<<(std::wostream& os) const
{
    os << m_text;
    return os;
}

int Verse::id(void) const 
{ 
    return m_id; 
}

int Verse::number(void) const 
{ 
    return m_number; 
}

bool Verse::rukuh(void) const 
{ 
    return m_rukuh; 
}

bool Verse::sajdah(void) const 
{ 
    return m_sajdah; 
}

bool Verse::manzil() const
{
    return m_manzil;
}

bool Verse::hizbQuarter() const
{
    return m_hizbQuarter;
}

const std::wstring& Verse::text(void) const
{ 
    return m_text; 
}

const quran::Chapter* Verse::chapter(void) const 
{ 
    return m_chapter; 
}

} // namespace quran
