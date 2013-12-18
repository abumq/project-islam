#ifndef DATA_HOLDER_HH
#define DATA_HOLDER_HH

#include "core/quran/quran.h"

class QApplication;
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
    void initialize(QApplication *app, QSplashScreen *splashScreen);
private:
    quran::Quran m_quranArabic;
    quran::Quran m_quranTransliteration;
    quran::Quran m_quranTranslation;
    quran::Quran m_quranTafseer;
};

} // namespace data
#endif // DATA_HOLDER_HH
