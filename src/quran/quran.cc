#include "quran/quran.hh"
#include "quran/chapter.hh"
#include "quran/verse.hh"
#include "db/database_manager.hh"

namespace quran {

Quran::Quran(void) : 
        m_ready(false) {
    load();
}

const quran::Verse* Quran::readVerse(quran::Chapter::Name chapter, int verseNumber) const {
    if (!m_ready)
        return nullptr;
    Chapters::const_iterator itChapter = m_chapters.find(static_cast<int>(chapter));
    if (itChapter != m_chapters.end()) {
        return itChapter->second.readVerse(verseNumber);
    }
    return nullptr;
}

const quran::Chapters& Quran::chapters(void) const { return m_chapters; }

const quran::Chapter* Quran::getChapter(int i) const {
    quran::Chapters::const_iterator it = m_chapters.find(i);
    if (it != m_chapters.end()) {
        return &it->second;
    }
    return nullptr;
}

bool Quran::ready(void) const { return m_ready; }

void Quran::load(void) {
    QURAN_LOG(DEBUG) << "Loading Quran...";
    TIMED_SCOPE(quranLoadTimer, "Load Complete Quran");
    db::DatabaseManager d;
    db::QueryResult result = d.query("SELECT * FROM QuranChapter ORDER BY ID");
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
        for (int i = 0; i < posList.size(); ++i)
            positions.push_back(posList.at(i).toInt());
        std::wstring muqattaat = result.at(i).value("Muqattaat").toString().toStdWString();
        quran::Chapter chapter(classification, name, std::move(arabicScriptName), 
                std::move(arabicName), std::move(englishName), rukuhCount, versesCount, sajdahCount, std::move(positions), std::move(muqattaat));
        m_chapters.insert(std::make_pair(cid, std::move(chapter)));
        quran::Chapter* chapterPtr = &m_chapters.at(cid);
        QVariantMap args;
        args.insert(":QuranChapterID", cid);
        db::QueryResult verses = d.query("SELECT * FROM QuranEnglish WHERE QuranChapterID = :QuranChapterID ORDER BY ID", args);
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


} // namespace quran
