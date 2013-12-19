#ifndef DATA_HOLDER_HH
#define DATA_HOLDER_HH

#include "core/quran/quran.h"

class QSplashScreen;

namespace data {

class DataHolder
{
public:
    DataHolder();
    quran::Quran* quranArabic();
    quran::Quran* quranTranslation();
    quran::Quran* quranTransliteration();
    quran::Quran* quranTafseer();
    void initialize(QSplashScreen *splashScreen);
private:
    quran::Quran m_quranArabic;
    quran::Quran m_quranTransliteration;
    quran::Quran m_quranTranslation;
    quran::Quran m_quranTafseer;
};

} // namespace data
#endif // DATA_HOLDER_HH
