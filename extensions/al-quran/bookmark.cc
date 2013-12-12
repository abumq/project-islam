#include "bookmark.h"
#include "core/logging/logging.h"
#include "core/quran/quran.h"

Bookmark::Bookmark() :
    m_name(""), m_chapter(1), m_verseFrom(1), m_verseTo(1)
{
    
}

Bookmark::Bookmark(const QString& name, int chapter, int verseFrom, int verseTo) :
    m_name(name), m_chapter(chapter), m_verseFrom(verseFrom), m_verseTo(verseTo)
{
    
}

QString Bookmark::name() const
{
    return m_name;
}

void Bookmark::setName(const QString& name)
{
    m_name = name;
}

int Bookmark::chapter() const
{
    return m_chapter;
}

void Bookmark::setChapter(int chapter)
{
    m_chapter = chapter;
}

int Bookmark::verseFrom() const
{
    return m_verseFrom;
}

void Bookmark::setVerseFrom(int verseFrom)
{
    m_verseFrom = verseFrom;
}

int Bookmark::verseTo() const
{
    return m_verseTo;
}

void Bookmark::setVerseTo(int verseTo)
{
    m_verseTo = verseTo;
}

QString Bookmark::serialize() const
{
    return m_name + "=" + QString::number(m_chapter) + ":" + 
            QString::number(m_verseFrom) + 
            (m_verseFrom < m_verseTo ? "-" + QString::number(m_verseTo) : "");
}

bool Bookmark::deserialize(const QString& serializedText)
{
    if (!serializedText.contains("=") || !serializedText.contains(":")) {
        LOG(ERROR) << "Invalid serialized text [" << serializedText << "]";
        return false;
    }
    QStringList tokens1 = serializedText.trimmed().split("=");
    if (tokens1.size() >= 2) {
        m_name = tokens1.at(0).trimmed();
        bool ok = false;
        QStringList tokens2 = tokens1.at(1).trimmed().split(":");
        if (tokens2.size() == 2) {
            // We expect at least format 
            // 1:1
            m_chapter = tokens2.at(0).trimmed().toInt(&ok);
            if (ok) {
                ok = m_chapter >= 1 && m_chapter <= quran::Quran::kChapterCount;
                QStringList tokensVerses = tokens2.at(1).split("-");
                m_verseFrom = tokensVerses.at(0).trimmed().toInt(&ok);
                if (ok && tokensVerses.size() >= 2) {
                    m_verseTo = tokensVerses.at(1).trimmed().toInt(&ok);
                }
            }
        }
        return ok;
    }
    return false;
}
