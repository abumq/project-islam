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


quran::Quran* DataHolder::quranEnglish()
{
    return &m_quranEnglish;
}

void DataHolder::initialize()
{
    quranArabic()->load(quran::Quran::TextType::Arabic);
    quranEnglish()->load(quran::Quran::TextType::English);
}

} // namespace data
