#ifndef BOOKMARKSBAR_H
#define BOOKMARKSBAR_H

#include <QWidget>
#include <QVector>
#include "bookmark.h"

class QListWidgetItem;
namespace Ui {
class BookmarksBar;
}

class BookmarksBar : public QWidget
{
    Q_OBJECT
    
public:
    static const int kBookmarkBarWidth = 200;
    BookmarksBar(const QString& settingsKeyPrefix, QWidget *parent = 0);
    ~BookmarksBar();
signals:
    void selectionChanged(Bookmark* bookmark);
public slots:
    void onSelectionChanged(QListWidgetItem* item);
private:
    Ui::BookmarksBar *ui;
    QString m_settingsKeyPrefix;
    QVector<Bookmark> m_bookmarks;
};

#endif // BOOKMARKSBAR_H
