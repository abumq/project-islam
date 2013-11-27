#include "core/quran/quran.h"
#include "core/quran/verse.h"
#include "core/quran/chapter.h"
#include "core/data/database_manager.h"

namespace quran {

const char* Quran::kQuranArabicDatabaseTable = "QuranArabic";
const char* Quran::kQuranChapterDatabaseTable = "QuranChapter";
const char* Quran::kQuranTransliterationDatabaseTable = "Quran__English_Transliteration";
const char* Quran::kQuranDefaultTranslationDatabaseTable = "Quran__English_Translation_Sahih_International";

Quran::Quran(void) : 
    m_ready(false) 
{
}

void Quran::load(const Quran::TextType &textType, const std::string& databaseTable)
{
    QURAN_LOG(DEBUG) << "Loading Quran...";
    TIMED_SCOPE(quranLoadTimer, "Load Complete Quran");
    m_textType = textType;
    data::DatabaseManager d("QuranDataManager", 
                            SettingsLoader().get(SettingsLoader::kSettingKeyQuranTranslationFile, "project-islam.db").toString());
    data::QueryResult result = d.query("SELECT * FROM " + QString(kQuranChapterDatabaseTable) + " ORDER BY ID");
    for (int i = 0; i < result.size(); ++i) {
        int cid = result.at(i).value("ID").toInt();
        quran::Chapter::Name name = static_cast<quran::Chapter::Name>(cid);
        quran::Chapter::Classification classification = static_cast<quran::Chapter::Classification>(result.at(i).value("Classification").toInt());
        std::wstring arabicScriptName = result.at(i).value("ArabicScriptName").toString().toStdWString();
        std::wstring arabicName = result.at(i).value("ArabicName").toString().toStdWString();
        std::wstring englishName = result.at(i).value("EnglishName").toString().toStdWString();
        int rukuhCount = result.at(i).value("RukuhCount").toInt();
        int versesCount = result.at(i).value("VersesCount").toInt();
        int sajdahCount = result.at(i).value("SajdahCount").toInt();
        QStringList posList = result.at(i).value("Position").toString().split(",");
        std::vector<int> positions;
        for (int j = 0; j < posList.size(); ++j)
            positions.push_back(posList.at(j).toInt());
        std::wstring muqattaat = result.at(i).value("Muqattaat").toString().toStdWString();
        quran::Chapter chapter(classification, name, std::move(arabicScriptName), 
                               std::move(arabicName), std::move(englishName), rukuhCount, versesCount, sajdahCount, std::move(positions), std::move(muqattaat));
        m_chapters.insert(std::make_pair(cid, std::move(chapter)));
        quran::Chapter* chapterPtr = &m_chapters.at(cid);
        QVariantMap args;
        args.insert(":QuranChapterID", cid);
        QString table;
        if (textType == TextType::Original) {
            table = kQuranArabicDatabaseTable;
            m_readingDirection = ReadingDirection::RightToLeft;
        } else if (textType == TextType::Translation || textType == TextType::Transliteration) {
            table = QString(databaseTable.empty() ? kQuranDefaultTranslationDatabaseTable : databaseTable.c_str());
            m_readingDirection = ReadingDirection::LeftToRight;
        }
        data::QueryResult verses = d.query("SELECT * FROM " + table + " WHERE QuranChapterID = :QuranChapterID ORDER BY ID", args);
        if (!d.lastQuerySuccessful()) {
            m_ready = false;
            return;
        }
        for (int i = 0; i < verses.size(); ++i) {
            int id = verses.at(i).value("ID").toInt();
            int number = verses.at(i).value("VerseNumber").toInt();
            std::wstring text = verses.at(i).value("VerseText").toString().toStdWString();
            bool rukuh = verses.at(i).value("Rukuh").toBool();
            bool sajdah  = verses.at(i).value("Sajdah").toBool();
            quran::Verse verse(id, number, std::move(text), chapterPtr, rukuh, sajdah);
            chapterPtr->m_verses.insert(std::make_pair(number, std::move(verse)));
        }
    }
    m_ready = true;
}

const quran::Verse* Quran::verse(quran::Chapter::Name chapter, int verseNumber) const 
{
    if (!m_ready)
        return nullptr;
    Chapters::const_iterator itChapter = m_chapters.find(static_cast<int>(chapter));
    if (itChapter != m_chapters.end()) {
        return itChapter->second.verse(verseNumber);
    }
    return nullptr;
}

const quran::Chapters& Quran::chapters(void) const { 
    return m_chapters; 
}

const quran::Chapter* Quran::chapter(Chapter::Name chapter) const 
{
    int cid = static_cast<int>(chapter);
    quran::Chapters::const_iterator it = m_chapters.find(cid);
    if (it != m_chapters.end()) {
        return &it->second;
    }
    return nullptr;
}

bool Quran::ready(void) const { 
    return m_ready; 
}

Quran::TextType Quran::textType() const
{
    return m_textType;
}

Quran::ReadingDirection Quran::readingDirection() const
{
    return m_readingDirection;
}


} // namespace quran
