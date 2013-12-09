#include "bookmarks_bar.h"
#include "ui_bookmarks_bar.h"
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QInputDialog>
#include "core/settings_loader.h"
#include "core/memory.h"
#include "core/quran/chapter.h"

const char* BookmarksBar::kBookmarkSeparator = ";";

BookmarksBar::BookmarksBar(const QString& settingsKeyPrefix, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookmarksBar),
    m_settingsKeyPrefix(settingsKeyPrefix)
{
    memory::turnToNullPtr(m_contextMenu, m_model, m_bookmarksList, m_addButton);
    ui->setupUi(this);
    
    m_bookmarksList = new BookmarksList(this);
    
    m_contextMenu = new QMenu();
    m_contextMenu->hide();
    
    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(2);
    m_model->setHorizontalHeaderLabels(QStringList() << "Name" << "Location");
    
    m_addButton = new QPushButton("+", this);
    m_addButton->resize(20, 20);
    m_addButton->hide();
    
    QObject::connect(m_addButton, SIGNAL(clicked()), this, SLOT(add()));
    
    ui->gridLayout->addWidget(m_addButton, 0, 0, 1, 1);
    ui->gridLayout->addWidget(m_bookmarksList, 1, 0, 1, 1);
    
    m_bookmarksList->setModel(m_model);
    m_bookmarksList->setContextMenuPolicy(Qt::CustomContextMenu);
    m_bookmarksList->setEditTriggers(BookmarksList::NoEditTriggers);
    
    load();
    
    QAction* actionOpen = m_contextMenu->addAction("Open");
    QObject::connect(actionOpen, SIGNAL(triggered()), this, SLOT(openSelected()));
    QAction* actionEdit = m_contextMenu->addAction("Edit");
    QObject::connect(actionEdit, SIGNAL(triggered()), this, SLOT(editSelected()));
    QAction* actionDelete = m_contextMenu->addAction("Delete");
    QObject::connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteSelected()));
    QObject::connect(m_bookmarksList, SIGNAL(doubleClicked(QModelIndex)),
                     this, SLOT(onSelectionChanged(QModelIndex)));
    QObject::connect(m_bookmarksList, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(onCustomContextMenuRequested(QPoint)));
}

BookmarksBar::~BookmarksBar()
{
    save();
    memory::deleteAll(m_contextMenu, ui);
}

void BookmarksBar::save()
{
    _TRACE;
    QString serializedForm;
    for (int i = 0; i < m_bookmarksList->model()->rowCount(); ++i) {
        BookmarkItem* item = static_cast<BookmarkItem*>(static_cast<QStandardItemModel*>(
                                                            m_bookmarksList->model())->item(i));
        Bookmark* bm = static_cast<Bookmark*>(item);
        LOG(INFO) << bm->serialize();
        serializedForm.append(item->serialize());
        if (i < m_bookmarksList->model()->rowCount() - 1) {
            serializedForm.append(kBookmarkSeparator);
        }
    }
    LOG(DEBUG) << "Saving bookmarks: " << serializedForm;
    SettingsLoader().saveSettings(m_settingsKeyPrefix + "bookmarks", serializedForm);
}

void BookmarksBar::load()
{
    _TRACE;
    m_model->clear();
    QString bookmarksStr = SettingsLoader().get(m_settingsKeyPrefix + "bookmarks").toString();
    if (!bookmarksStr.isEmpty()) {
        QStringList allBookmarksStr = bookmarksStr.split(kBookmarkSeparator);
        for (QString bookmarkStr : allBookmarksStr) {
            add(bookmarkStr);
        }
    }
    // Only connect this signal when bookmarks are loaded
    QObject::connect(m_model, SIGNAL(itemChanged(QStandardItem*)), 
                     this, SLOT(onItemChanged(QStandardItem*)));
}

bool BookmarksBar::add(const QString& bookmarkStr) 
{
    Bookmark bm;
    if (bm.deserialize(bookmarkStr)) {
        LOG(INFO) << "Adding bookmark [" << bm << "]";
        BookmarkItem* bookmarkItem = new BookmarkItem(
                    bm.serialize().mid(bm.name().length() + 1 /* name= */)
                    );
        bookmarkItem->setText(bm.name());
        bookmarkItem->setName(bm.name());
        bookmarkItem->setChapter(bm.chapter());
        bookmarkItem->setVerseFrom(bm.verseFrom());
        bookmarkItem->setVerseTo(bm.verseTo());
        
        int currRow = m_model->rowCount();
        m_model->setItem(currRow, 0, bookmarkItem);
        m_model->setItem(currRow, 1, bookmarkItem->locationItem());
        return true;
    }
    return false;
}

void BookmarksBar::add()
{
    QInputDialog nameDialog(this);
    nameDialog.setWindowTitle("Enter bookmark name:");
    nameDialog.exec();
    QString name = nameDialog.textValue().replace(kBookmarkSeparator, ",");
    QString serializedText = name + "=" + m_currentJumpText;
    if (add(serializedText)) {
        save();
    } else {
        LOG(ERROR) << "Unable to deserialize: " << serializedText;
    }
}

void BookmarksBar::onSelectionChanged(const QModelIndex& modelIndex)
{
    _TRACE;
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
    _TRACE;
    m_contextMenu->exec(QCursor::pos());
}

void BookmarksBar::openSelected()
{
    _TRACE;
    onSelectionChanged(m_bookmarksList->currentIndex());
}

void BookmarksBar::editSelected()
{
    _TRACE;
    BookmarkItem* selectedItem = m_bookmarksList->selectedItem();
    if (selectedItem == nullptr) {
        return;
    }
    m_bookmarksList->edit(m_bookmarksList->currentIndex());
}

void BookmarksBar::deleteSelected()
{
    _TRACE;
    m_bookmarksList->model()->removeRow(m_bookmarksList->currentIndex().row());
}

void BookmarksBar::onItemChanged(QStandardItem* item)
{
    _TRACE;
    if (item == nullptr) {
        return;
    }
    if (item->index().column() == 1) {
        LOG(DEBUG) << "Editing location to [" << item->text() << "]";
        BookmarkItem* bm = static_cast<BookmarkItem*>(
                    item->model()->item(item->index().row(), 0));
        QString deserializeText = bm->name() + "=" + item->text();
        if (!bm->deserialize(deserializeText)) {
            
        }
    } else {
        BookmarkItem* bm = static_cast<BookmarkItem*>(item);
        LOG(DEBUG) << "Editing name to [" << bm->text() << "]";
        bm->setName(bm->text());
    }
}
QString BookmarksBar::currentJumpText() const
{
    return m_currentJumpText;
}

void BookmarksBar::setCurrentJumpText(const QString& currentJumpText)
{
    _TRACE;
    if (!currentJumpText.isEmpty()) {
        m_addButton->show();
        m_addButton->setToolTip("Add [" + currentJumpText + "] to bookmarks");
    }
    m_currentJumpText = currentJumpText;
}

