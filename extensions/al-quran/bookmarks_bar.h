#ifndef BOOKMARKSBAR_H
#define BOOKMARKSBAR_H

#include <QWidget>
#include <QVector>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include "core/memory.h"
#include "bookmark.h"

class QMenu;
namespace Ui {
class BookmarksBar;
}

class BookmarkItem : public QStandardItem, public Bookmark {
public:
    BookmarkItem(const QString& locationStr) :
        m_location(new QStandardItem(locationStr)) {
    }
    
    QStandardItem* locationItem() {
        return m_location;
    }
    
    virtual ~BookmarkItem() {
        memory::deleteAll(m_location);
    }

private:
    QStandardItem* m_location;
};

class BookmarksList : public QTreeView {
    Q_OBJECT
public:
    BookmarksList(QWidget* parent) :
        QTreeView(parent) {
    }
    
    BookmarkItem* selectedItem() {
        return static_cast<BookmarkItem*>(
                    static_cast<QStandardItemModel*>(
                        model())->itemFromIndex(currentIndex()));
    }
};

class BookmarksBar : public QWidget
{
    Q_OBJECT
    
public:
    static const int kBookmarkBarWidth = 400;
    BookmarksBar(const QString& settingsKeyPrefix, QWidget *parent = 0);
    ~BookmarksBar();
    void save();
    void load();
signals:
    void selectionChanged(Bookmark* bookmark);
public slots:
    void onSelectionChanged(const QModelIndex&);
    void onCustomContextMenuRequested(const QPoint &);
    void openSelected();
    void editSelected();
    void deleteSelected();
    void onItemChanged(QStandardItem*);
private:
    Ui::BookmarksBar *ui;
    BookmarksList* m_bookmarksList;
    QMenu* m_contextMenu;
    QStandardItemModel* m_model;
    QString m_settingsKeyPrefix;
};

#endif // BOOKMARKSBAR_H
