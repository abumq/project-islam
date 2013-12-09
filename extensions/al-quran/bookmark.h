#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QString>
#include "core/logging.h"

class Bookmark : public el::Loggable
{
public:
    static const int kBookmarkNameLength = 30;
    
    Bookmark();
    Bookmark(const QString& name, int chapter, int verseFrom, int verseTo);
    QString name() const;
    void setName(const QString& name);
    
    int chapter() const;
    void setChapter(int chapter);
    
    int verseFrom() const;
    void setVerseFrom(int verseFrom);
    
    int verseTo() const;
    void setVerseTo(int verseTo);
    
    QString serialize() const;
    bool deserialize(const QString& serializedText);
    
    void log(el::base::type::ostream_t &) const;
    bool operator==(const Bookmark& other) {
        return serialize() == other.serialize();
    }

private:
    QString m_name;
    int m_chapter;
    int m_verseFrom;
    int m_verseTo;
};

#endif // BOOKMARK_H
