#ifndef DATA_HOLDER_HH
#define DATA_HOLDER_HH

#include "core/quran/quran.h"

namespace data {

class DataHolder
{
public:
    DataHolder();
    quran::Quran* quranArabic();
    quran::Quran* quranTranslation();
    quran::Quran* quranTransliteration();
    void initialize();
private:
    quran::Quran m_quranArabic;
    quran::Quran m_quranTransliteration;
    quran::Quran m_quranTranslation;
};

} // namespace data
#endif // DATA_HOLDER_HH
