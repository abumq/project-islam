#ifndef BOOKMARKSBAR_H
#define BOOKMARKSBAR_H

#include <QWidget>
#include <QVector>
#include "bookmark.h"

class QTreeWidgetItem;
class QStandardItemModel;
class QMenu;
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
    void onSelectionChanged(const QModelIndex&);
    void onCustomContextMenuRequested(const QPoint &pos);
    void openSelected();
    void editSelected();
    void deleteSelected();
private:
    Ui::BookmarksBar *ui;
    QMenu* m_contextMenu;
    QStandardItemModel* m_model;
    QString m_settingsKeyPrefix;
    QVector<Bookmark> m_bookmarks;
};

#endif // BOOKMARKSBAR_H
