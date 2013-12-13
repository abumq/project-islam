#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QString>
#include <ostream>

class Bookmark
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

    int selected() const;
    void setSelected(int selected);
    
    bool operator==(const Bookmark& other) {
        return serialize() == other.serialize();
    }
    
private:
    QString m_name;
    int m_chapter;
    int m_verseFrom;
    int m_verseTo;
    int m_selected;
};

#endif // BOOKMARK_H
