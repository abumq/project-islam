#ifndef DATA_HOLDER_HH
#define DATA_HOLDER_HH

#include "core/quran/quran.h"

namespace data {
class DataHolder
{
public:
    DataHolder();
    quran::Quran* quranArabic();
    quran::Quran* quranEnglish();
private:
    quran::Quran m_quranArabic;
    quran::Quran m_quranEnglish;
};
} // namespace database
#endif // DATA_HOLDER_HH
