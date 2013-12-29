#ifndef _LOGGER
#   define _LOGGER "data"
#endif
#ifndef _PERFORMANCE_LOGGER
#   define _PERFORMANCE_LOGGER _LOGGER
#endif
#include "core/data/data_holder.h"
#include <QApplication>
#include <QSplashScreen>
#include "core/logging/logging.h"

namespace data {

DataHolder::DataHolder()
{
    _TRACE;
}

quran::Quran* DataHolder::quranArabic()
{
    return &m_quranArabic;
}

quran::Quran* DataHolder::quranTransliteration()
{
    return &m_quranTransliteration;
}

quran::Quran* DataHolder::quranTafseer()
{
    return &m_quranTafseer;
}

quran::Quran* DataHolder::quranTranslation()
{
    return &m_quranTranslation;
}

void DataHolder::initialize(QSplashScreen *splashScreen)
{
    if (splashScreen != nullptr) {
        splashScreen->showMessage("Loading Qur'an...", Qt::AlignHCenter | Qt::AlignBottom);
        qApp->processEvents();
    }
    std::string quranArabicTable = SettingsLoader::getInstance().get(QString(SettingsLoader::kSettingKeyQuranTable), 
                                             QString(quran::Quran::kQuranDefaultArabicDatabaseTable)).toString().toStdString();
    quranArabic()->load(quran::Quran::TextType::Original, quranArabicTable);
    CHECK(quranArabic()->ready()) << "Unable to load Quran Arabic from [" << quranArabicTable << "]";
    if (splashScreen != nullptr) {
        splashScreen->showMessage("Loading Qur'an... [25%]", Qt::AlignHCenter | Qt::AlignBottom);
        qApp->processEvents();
    }
    std::string quranTranslationTable = SettingsLoader::getInstance().get(QString(SettingsLoader::kSettingKeyQuranTranslationTable), 
                                                  QString(quran::Quran::kQuranDefaultTranslationDatabaseTable)).toString().toStdString();
    quranTranslation()->load(quran::Quran::TextType::Translation, quranTranslationTable);
    CHECK(quranArabic()->ready()) << "Unable to load Quran Translation from [" << quranTranslationTable << "]";
    if (splashScreen != nullptr) {
        splashScreen->showMessage("Loading Qur'an... [50%]", Qt::AlignHCenter | Qt::AlignBottom);
        qApp->processEvents();
    }
    
    std::string quranTransliterationTable = SettingsLoader::getInstance().get(QString(SettingsLoader::kSettingKeyQuranTransliterationTable), 
                                                      QString(quran::Quran::kQuranDefaultTransliterationDatabaseTable)).toString().toStdString();
    quranTransliteration()->load(quran::Quran::TextType::Transliteration, quranTransliterationTable);
    CHECK(quranArabic()->ready()) << "Unable to load Quran Transliteration from [" << quranTransliterationTable << "]";
    if (splashScreen != nullptr) {
        splashScreen->showMessage("Loading Qur'an... [75%]", Qt::AlignHCenter | Qt::AlignBottom);
        qApp->processEvents();
    }
    std::string quranTafseerTable = SettingsLoader::getInstance().get(QString(SettingsLoader::kSettingKeyQuranTafseerTable), 
                                              QString(quran::Quran::kQuranDefaultTafseerDatabaseTable)).toString().toStdString();
    quranTafseer()->load(quran::Quran::TextType::Tafseer, quranTafseerTable);
    CHECK(quranArabic()->ready()) << "Unable to load Quran Transliteration from [" << quranTafseerTable << "]";
    if (splashScreen != nullptr) {
        splashScreen->showMessage("Loading Qur'an... [100%]", Qt::AlignHCenter | Qt::AlignBottom);
        qApp->processEvents();
    }
}

} // namespace data
