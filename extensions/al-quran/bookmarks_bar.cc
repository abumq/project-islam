#include "bookmarks_bar.h"
#include "ui_bookmarks_bar.h"
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include "core/settings_loader.h"
#include "core/memory.h"
#include "core/quran/chapter.h"

BookmarksBar::BookmarksBar(const QString& settingsKeyPrefix, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookmarksBar),
    m_contextMenu(nullptr),
    m_model(nullptr),
    m_settingsKeyPrefix(settingsKeyPrefix)
{
    ui->setupUi(this);
    
    m_bookmarksList = new BookmarksList(this);
    ui->gridLayout->addWidget(m_bookmarksList, 0, 0, 1, 1);
    
    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(2);
    m_model->setHorizontalHeaderLabels(QStringList() << "Name" << "Location");
    m_bookmarksList->setModel(m_model);
    m_bookmarksList->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Load bookmarks
    QString bookmarksStr = SettingsLoader().get(m_settingsKeyPrefix + "bookmarks").toString();
    if (!bookmarksStr.isEmpty()) {
        QStringList allBookmarksStr = bookmarksStr.split(",");
        for (QString bookmarkStr : allBookmarksStr) {
            Bookmark bm;
            if (bm.deserialize(bookmarkStr)) {
                LOG(INFO) << "Loading bookmark [" << bm << "]";
                BookmarkItem* name = new BookmarkItem;
                name->setText(bm.name());
                name->setName(bm.name());
                name->setChapter(bm.chapter());
                name->setVerseFrom(bm.verseFrom());
                name->setVerseTo(bm.verseTo());
                QString locationStr = bm.serialize().mid(bm.name().length() + 1 /* name= */);
                QStandardItem* location = new QStandardItem(locationStr);
                m_bookmarksList->setEditTriggers(BookmarksList::NoEditTriggers);
                
                int currRow = m_model->rowCount();
                m_model->setItem(currRow, 0, name);
                m_model->setItem(currRow, 1, location);
            }
        }
    }
    QObject::connect(m_bookmarksList, SIGNAL(doubleClicked(QModelIndex)),
                     this, SLOT(onSelectionChanged(QModelIndex)));
    QObject::connect(m_bookmarksList, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(onCustomContextMenuRequested(QPoint)));
    m_contextMenu = new QMenu();
    QAction* actionOpen = m_contextMenu->addAction("Open");
    QObject::connect(actionOpen, SIGNAL(triggered()), this, SLOT(openSelected()));
    QAction* actionEdit = m_contextMenu->addAction("Edit");
    QObject::connect(actionEdit, SIGNAL(triggered()), this, SLOT(editSelected()));
    QAction* actionDelete = m_contextMenu->addAction("Delete");
    QObject::connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteSelected()));
    m_contextMenu->hide();
}

BookmarksBar::~BookmarksBar()
{
    // Save
    QString serializedForm;
    for (int i = 0; i < m_bookmarksList->model()->rowCount(); ++i) {
        BookmarkItem* item = static_cast<BookmarkItem*>(static_cast<QStandardItemModel*>(
                                                  m_bookmarksList->model())->item(i));
        Bookmark* bm = static_cast<Bookmark*>(item);
        LOG(INFO) << bm->serialize();
        // FIXME : This will work when we sync item->name with item->text
        serializedForm.append(item->serialize());
        if (i < m_bookmarksList->model()->rowCount() - 1) {
            serializedForm.append(",");
        }
    }
    LOG(DEBUG) << "Saving bookmarks to: " << serializedForm;
    SettingsLoader().saveSettings(m_settingsKeyPrefix + "bookmarks", serializedForm);
    memory::deleteAll(m_contextMenu, ui);
}

void BookmarksBar::onSelectionChanged(const QModelIndex& modelIndex)
{
    for (int i = 0; i < m_bookmarksList->model()->rowCount(); ++i) {
        BookmarkItem* item = static_cast<BookmarkItem*>(static_cast<QStandardItemModel*>(
                                                  m_bookmarksList->model())->item(i));
        Bookmark* bm = static_cast<Bookmark*>(item);
        QString serialized = modelIndex.sibling(modelIndex.row(), 0).data().toString() + "=" +
                modelIndex.sibling(modelIndex.row(), 1).data().toString();
        if (bm->serialize() == serialized) {
            emit selectionChanged(bm);
        }
    }
}

void BookmarksBar::onCustomContextMenuRequested(const QPoint&)
{
    m_contextMenu->exec(QCursor::pos());
}

void BookmarksBar::openSelected()
{
    onSelectionChanged(m_bookmarksList->currentIndex());
}

void BookmarksBar::editSelected()
{
    BookmarkItem* selectedItem = m_bookmarksList->selectedItem();
    if (selectedItem == nullptr) {
        return;
    }
    m_bookmarksList->edit(m_bookmarksList->currentIndex());
}

void BookmarksBar::deleteSelected()
{
    m_bookmarksList->model()->removeRow(m_bookmarksList->currentIndex().row());
}
