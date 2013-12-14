#ifndef _LOGGER
#   define _LOGGER "data"
#endif
#ifndef _PERFORMANCE_LOGGER
#   define _PERFORMANCE_LOGGER _LOGGER
#endif
#include "core/data/data_holder.h"
#include "core/logging/logging.h"

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

quran::Quran* DataHolder::quranTafseer()
{
    return &m_quranTafseer;
}

quran::Quran* DataHolder::quranTranslation()
{
    return &m_quranTranslation;
}

void DataHolder::initialize()
{
    quranArabic()->load(quran::Quran::TextType::Original, quran::Quran::kQuranArabicDatabaseTable);
    
    quranTranslation()->load(quran::Quran::TextType::Translation, 
                             SettingsLoader().get(QString(SettingsLoader::kSettingKeyQuranTranslationTable), 
                                                  QString(quran::Quran::kQuranDefaultTranslationDatabaseTable)).toString().toStdString());
    
    quranTransliteration()->load(quran::Quran::TextType::Transliteration, quran::Quran::kQuranTransliterationDatabaseTable);
    quranTranslation()->load(quran::Quran::TextType::Tafseer, 
                             SettingsLoader().get(QString(SettingsLoader::kSettingKeyQuranTafseerTable), 
                                                  QString(quran::Quran::kQuranDefaultTafseerDatabaseTable)).toString().toStdString());
    
}

} // namespace data
