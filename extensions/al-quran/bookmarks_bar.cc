#include "bookmarks_bar.h"
#include "ui_bookmarks_bar.h"
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QInputDialog>
#include <QSplitter>
#include "core/logging/logging.h"
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
    
    m_contextMenu = new QMenu();
    m_contextMenu->hide();
    
    m_model = new QStandardItemModel(this);
    
    m_bookmarksList = new BookmarksList(this);
    m_bookmarksList->setModel(m_model);
    m_bookmarksList->setContextMenuPolicy(Qt::CustomContextMenu);
    m_bookmarksList->setEditTriggers(BookmarksList::NoEditTriggers);
    
    QFont font;
    font.setFamily(QStringLiteral("FreeSerif"));
    font.setPointSize(16);
    font.setBold(true);
    font.setWeight(75);
    
    m_addButton = new QPushButton(QIcon(":/icons/add"), "", this);
    m_addButton->resize(20, 20);
    m_addButton->setEnabled(false);
    m_addButton->setFont(font);
    
    m_mergeButton = new QPushButton(QIcon(":/icons/merge"), "", this);
    m_mergeButton->resize(20, 20);
    m_mergeButton->setEnabled(false);
    m_mergeButton->setFont(font);
    
    QObject::connect(m_addButton, SIGNAL(clicked()), this, SLOT(add()));
    QObject::connect(m_mergeButton, SIGNAL(clicked()), this, SLOT(merge()));
    
    ui->gridLayout->addWidget(new QSplitter(this), 0, 0, 1, 1);
    ui->gridLayout->addWidget(m_mergeButton, 0, 1, 1, 1);
    ui->gridLayout->addWidget(m_addButton, 0, 3, 1, 1);
    ui->gridLayout->addWidget(m_bookmarksList, 1, 0, 1, 4);
    
    load();
    
    QAction* actionOpen = m_contextMenu->addAction("Open");
    QObject::connect(actionOpen, SIGNAL(triggered()), 
                     this, SLOT(openSelected()));
    QAction* actionEdit = m_contextMenu->addAction("Edit");
    QObject::connect(actionEdit, SIGNAL(triggered()), 
                     this, SLOT(editSelected()));
    QAction* actionDelete = m_contextMenu->addAction("Delete");
    QObject::connect(actionDelete, SIGNAL(triggered()), 
                     this, SLOT(deleteSelected()));
    QObject::connect(m_bookmarksList, SIGNAL(activated(QModelIndex)),
                     this, SLOT(onActivated(QModelIndex)));             
    QObject::connect(m_bookmarksList, SIGNAL(doubleClicked(QModelIndex)),
                     this, SLOT(onSelectionChanged(QModelIndex)));
    QObject::connect(m_bookmarksList, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(onCustomContextMenuRequested(QPoint)));
    m_contextMenu->addSeparator();
    QAction* actionMoveUp = m_contextMenu->addAction("Move Up");
    actionMoveUp->setObjectName("actionMoveUp");
    QObject::connect(actionMoveUp, SIGNAL(triggered()), 
                     this, SLOT(moveUp()));
    QAction* actionMoveDown = m_contextMenu->addAction("Move Down");
    actionMoveDown->setObjectName("actionMoveDown");
    QObject::connect(actionMoveDown, SIGNAL(triggered()), 
                     this, SLOT(moveDown()));
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
    QStringList columnHeaders = QStringList() << "Name" << "Location";
    m_model->setColumnCount(columnHeaders.size());
    m_model->setHorizontalHeaderLabels(columnHeaders);
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
    _TRACE;
    Bookmark bm;
    if (bm.deserialize(bookmarkStr)) {
        LOG(INFO) << "Adding bookmark [" << bm.serialize() << "]";
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
    _TRACE;
    QInputDialog nameDialog(this);
    nameDialog.setWindowTitle("Add Bookmark");
    nameDialog.setTextValue("Bookmark");
    nameDialog.setLabelText("Enter bookmark name:");
    if (nameDialog.exec() == QInputDialog::Rejected) {
        return;
    }
    QString name = nameDialog.textValue().replace(kBookmarkSeparator, ",");
    if (name.isEmpty()) {
        return;
    }
    name = name.mid(0, Bookmark::kBookmarkNameLength);
    QString serializedText = name + "=" + m_currentJumpText;
    if (add(serializedText)) {
        save();
    } else {
        LOG(ERROR) << "Unable to deserialize: " << serializedText;
    }
}

void BookmarksBar::merge()
{
    BookmarkItem* curr = static_cast<BookmarkItem*>(m_model->item(m_bookmarksList->currentIndex().row()));
    LOG(INFO) << curr;
}

void BookmarksBar::moveUp()
{
    _TRACE;
    QStandardItemModel* model = static_cast<QStandardItemModel*>(m_bookmarksList->model());
    int currRow = m_bookmarksList->currentIndex().row();
    BookmarkItem* itemName = static_cast<BookmarkItem*>(model->takeItem(currRow, 0));
    QStandardItem* itemLoc = model->takeItem(currRow, 1);
    BookmarkItem* item2Name = static_cast<BookmarkItem*>(model->takeItem(currRow - 1, 0));
    QStandardItem* item2Loc = model->takeItem(currRow - 1, 1);
    m_model->setItem(currRow, 0, item2Name);
    m_model->setItem(currRow , 1, item2Loc);
    m_model->setItem(currRow - 1, 0, itemName);
    m_model->setItem(currRow - 1, 1, itemLoc);
}

void BookmarksBar::moveDown()
{
    _TRACE;
    QStandardItemModel* model = static_cast<QStandardItemModel*>(m_bookmarksList->model());
    int currRow = m_bookmarksList->currentIndex().row();
    BookmarkItem* itemName = static_cast<BookmarkItem*>(model->takeItem(currRow, 0));
    QStandardItem* itemLoc = model->takeItem(currRow, 1);
    BookmarkItem* item2Name = static_cast<BookmarkItem*>(model->takeItem(currRow + 1, 0));
    QStandardItem* item2Loc = model->takeItem(currRow + 1, 1);
    m_model->setItem(currRow, 0, item2Name);
    m_model->setItem(currRow , 1, item2Loc);
    m_model->setItem(currRow + 1, 0, itemName);
    m_model->setItem(currRow + 1, 1, itemLoc);
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

void BookmarksBar::onActivated(const QModelIndex& modelIndex)
{
    if (!currentJumpText().isEmpty()) {
        BookmarkItem* selection = static_cast<BookmarkItem*>(m_model->item(modelIndex.row()));
        m_mergeButton->setToolTip("Merge [" + currentJumpText() + "] to " + selection->name());
        m_mergeButton->setEnabled(true);
    } else {
        m_mergeButton->setEnabled(false);
    }
}

void BookmarksBar::onCustomContextMenuRequested(const QPoint&)
{
    _TRACE;
    for (QAction* act : m_contextMenu->actions()) {
        act->setEnabled(true);
    }
    if (m_bookmarksList->currentIndex().row() == 0) {
        for (QAction* act : m_contextMenu->actions()) {
            if (act->objectName() == "actionMoveUp") {
                act->setEnabled(false);
            }
        }
    } else if (m_bookmarksList->currentIndex().row() == 
               m_bookmarksList->model()->rowCount() - 1) {
        for (QAction* act : m_contextMenu->actions()) {
            if (act->objectName() == "actionMoveDown") {
                act->setEnabled(false);
            }
        }
    }
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
    BookmarkItem* bm;
    if (item->index().column() == 1) {
        LOG(DEBUG) << "Editing location to [" << item->text() << "]";
        bm = static_cast<BookmarkItem*>(
                    item->model()->item(item->index().row(), 0));
        QString deserializeText = bm->name() + "=" + item->text();
        if (!bm->deserialize(deserializeText)) {
            
        }
    } else {
        bm = static_cast<BookmarkItem*>(item);
        LOG(DEBUG) << "Editing name to [" << bm->text() << "]";
        bm->setName(bm->text());
    }
}
QString BookmarksBar::currentJumpText() const
{
    _TRACE;
    return m_currentJumpText;
}

void BookmarksBar::setCurrentJumpText(const QString& currentJumpText)
{
    _TRACE;
    m_currentJumpText = currentJumpText;
    if (!currentJumpText.isEmpty()) {
        m_addButton->setToolTip("Add [" + currentJumpText + "] to bookmarks");
        m_addButton->setEnabled(true);
    }
    onActivated(m_bookmarksList->currentIndex());
}

