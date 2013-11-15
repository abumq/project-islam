#include "quran/verse.hh"

namespace quran {

Verse::Verse(int id, int number, std::wstring&& text, Chapter* chapter, bool rukuh, bool sajdah) :
        m_id(id), m_number(number), m_text(std::move(text)), m_chapter(chapter), m_rukuh(rukuh), m_sajdah(sajdah) {
}

void Verse::log(el::base::type::ostream_t& os) const {
    os << m_text;
}

int Verse::id(void) const { return m_id; }

int Verse::number(void) const { return m_number; }

bool Verse::rukuh(void) const { return m_rukuh; }

bool Verse::sajdah(void) const { return m_sajdah; }

const std::wstring& Verse::text(void) const { return m_text; }

const quran::Chapter* Verse::chapter(void) const { return m_chapter; }

} // namespace quran
