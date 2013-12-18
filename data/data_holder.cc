#ifndef _LOGGER
#   define _LOGGER "data"
#endif
#ifndef _PERFORMANCE_LOGGER
#   define _PERFORMANCE_LOGGER _LOGGER
#endif
#include "data/data_holder.h"
#include <QApplication>
#include <QSplashScreen>
#include "logging/logging.h"

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

void DataHolder::initialize(QApplication* app, QSplashScreen *splashScreen)
{
    if (splashScreen != nullptr) {
        splashScreen->showMessage("Loading Qur'an...", Qt::AlignHCenter | Qt::AlignBottom);
    }
    quranArabic()->load(quran::Quran::TextType::Original, 
                        SettingsLoader().get(QString(SettingsLoader::kSettingKeyQuranTable), 
                                             QString(quran::Quran::kQuranDefaultArabicDatabaseTable)).toString().toStdString());
    if (splashScreen != nullptr) {
        splashScreen->showMessage("Loading Qur'an... [25%]", Qt::AlignHCenter | Qt::AlignBottom);
    }
    app->processEvents();
    quranTranslation()->load(quran::Quran::TextType::Translation, 
                             SettingsLoader().get(QString(SettingsLoader::kSettingKeyQuranTranslationTable), 
                                                  QString(quran::Quran::kQuranDefaultTranslationDatabaseTable)).toString().toStdString());
    if (splashScreen != nullptr) {
        splashScreen->showMessage("Loading Qur'an... [50%]", Qt::AlignHCenter | Qt::AlignBottom);
    }
    app->processEvents();
    quranTransliteration()->load(quran::Quran::TextType::Transliteration, 
                                 SettingsLoader().get(QString(SettingsLoader::kSettingKeyQuranTransliterationTable), 
                                                      QString(quran::Quran::kQuranDefaultTransliterationDatabaseTable)).toString().toStdString());
    if (splashScreen != nullptr) {
        splashScreen->showMessage("Loading Qur'an... [75%]", Qt::AlignHCenter | Qt::AlignBottom);
    }
    app->processEvents();
    quranTafseer()->load(quran::Quran::TextType::Tafseer, 
                         SettingsLoader().get(QString(SettingsLoader::kSettingKeyQuranTafseerTable), 
                                              QString(quran::Quran::kQuranDefaultTafseerDatabaseTable)).toString().toStdString());
    if (splashScreen != nullptr) {
        splashScreen->showMessage("Loading Qur'an... [100%]", Qt::AlignHCenter | Qt::AlignBottom);
    }
}

} // namespace data
