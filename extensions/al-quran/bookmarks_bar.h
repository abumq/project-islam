#ifndef BOOKMARKSBAR_H
#define BOOKMARKSBAR_H

#include <QWidget>
#include <QVector>
#include "bookmark.h"
#include <QTreeView>
#include <QStandardItemModel>

class QStandardItem;
class QMenu;
namespace Ui {
class BookmarksBar;
}

class BookmarksList : public QTreeView {
    Q_OBJECT
public:
    BookmarksList(QWidget* parent) :
        QTreeView(parent) {
    }
    
    QStandardItem* selectedItem() {
        return static_cast<QStandardItemModel*>(model())->itemFromIndex(currentIndex());
    }
};

class BookmarksBar : public QWidget
{
    Q_OBJECT
    
public:
    static const int kBookmarkBarWidth = 400;
    BookmarksBar(const QString& settingsKeyPrefix, QWidget *parent = 0);
    ~BookmarksBar();
signals:
    void selectionChanged(Bookmark* bookmark);
public slots:
    void onSelectionChanged(const QModelIndex&);
    void onCustomContextMenuRequested(const QPoint &);
    void openSelected();
    void editSelected();
    void deleteSelected();
private:
    Ui::BookmarksBar *ui;
    BookmarksList* m_bookmarksList;
    QMenu* m_contextMenu;
    QStandardItemModel* m_model;
    QString m_settingsKeyPrefix;
    QVector<Bookmark> m_bookmarks;
};

#endif // BOOKMARKSBAR_H
