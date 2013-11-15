#include "quran/quran_reader.hh"
#include "quran/chapter.hh"
#include <QTextCursor>
#include <QTextCharFormat>
#include <QScrollBar>

namespace quran {
namespace ui {

const QString QuranReader::kSajdahText = "<b>۩</b>";

QuranReader::QuranReader(QObject *parent)
    : QTextDocument(parent)
{
}

void QuranReader::update(const quran::Chapter* chapter)
{
    QString versesHtml;
    for (quran::Verses::const_iterator it = chapter->verses().begin(); it != chapter->verses().end(); ++it) {
        const quran::Verse* verse = &it->second;
        versesHtml.append("(<a name='verse_" + QString::number(verse->number()) + "'>" + QString::number(verse->number()) + "</a>) " + QString::fromStdWString(verse->text()));
        if (verse->sajdah()) {
            versesHtml.append(kSajdahText);
        }
        versesHtml.append("<br/><br/>");
    }
    m_currentHtml = versesHtml;
    m_currentChapter = const_cast<quran::Chapter*>(chapter);
    setHtml(versesHtml);
    
    changeSize(kMinSize);
}

QString QuranReader::highlightVerse(int verseNumber)
{
    QString verse = QString::fromStdWString(m_currentChapter->readVerse(verseNumber)->text());
    return find("(" + QString::number(verseNumber) + ") " + verse);
}

QString QuranReader::find(const QString& str)
{
    LOG(INFO) << "Finding [" << str << "]";
    TIMED_SCOPE(findTimer, "Found [" + str.toStdString() + "]");
    
    TIMED_BLOCK(block1, "setHtml") {
        clear();
        // Takes ~20 sec.. why???
        setHtml(m_currentHtml);
    }
    
    QTextCursor highlightCursor(this);
    QTextCursor cursor(this);
    cursor.beginEditBlock();
    
    QTextCharFormat plainFormat = highlightCursor.charFormat();
    QTextCharFormat colorFormat = plainFormat;
    colorFormat.setBackground(Qt::yellow);
    highlightCursor = QTextDocument::find(str);
    if (!highlightCursor.isNull()) {
        highlightCursor.movePosition(QTextCursor::EndOfWord,
                                     QTextCursor::KeepAnchor, str.size());
        highlightCursor.mergeCharFormat(colorFormat);
    }
    cursor.endEditBlock();
    
    QTextCursor cursorSel = highlightCursor;
    cursorSel.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor, str.size());
    QString verseAnchor = cursorSel.selectedText();
    if (verseAnchor.contains('(') && verseAnchor.contains(')') && verseAnchor.startsWith('(')) {
        verseAnchor = "verse_" + verseAnchor.mid(1, verseAnchor.indexOf(")") - 1) ;
    }
    return verseAnchor;
}

void QuranReader::sizeUp(unsigned int threshold)
{
    m_size += threshold;
    if (m_size > kMaxSize) {
        m_size = kMaxSize;
        return;
    }
    changeSize(m_size);
}

void QuranReader::sizeDown(unsigned int threshold)
{
    m_size -= threshold;
    if (m_size < kMinSize) {
        m_size = kMinSize;
        return;
    }
    changeSize(m_size);
}

void QuranReader::changeSize(unsigned int size)
{
    LOG(INFO) << "Changing size [" << size << "]";
    m_size = size;
    QTextCursor cursor(this);
    cursor.select(QTextCursor::Document);
    QTextCharFormat fmt = cursor.charFormat();
    QFont font = fmt.font();
    font.setPointSize(m_size);
    fmt.setFont(font);
    cursor.mergeCharFormat(fmt);
    cursor.clearSelection();
    cursor.endEditBlock();
}

} // namespace ui
} // namespace quran
