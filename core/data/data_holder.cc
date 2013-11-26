#include "core/data/data_holder.h"
#include "core/logging.h"

namespace data {

DataHolder::DataHolder()
{
    _TRACE;
    initialize();
}

quran::Quran* DataHolder::quranArabic()
{
    return &m_quranArabic;
}

quran::Quran* DataHolder::quranTransliteration()
{
    return &m_quranTransliteration;
}

quran::Quran* DataHolder::quranTranslation()
{
    return &m_quranTranslation;
}

void DataHolder::initialize()
{
    quranArabic()->load(quran::Quran::TextType::Original);
    // Default is English (Yusuf Ali)
    quranTranslation()->load(quran::Quran::TextType::Translation);
    // TODO: Build transliteration database and specify database table here
    quranTransliteration()->load(quran::Quran::TextType::Transliteration);
}

} // namespace data
