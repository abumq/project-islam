#include "bookmarks_bar.h"
#include "ui_bookmarks_bar.h"
#include <QStandardItemModel>
#include "core/settings_loader.h"
#include "core/memory.h"
#include "core/quran/chapter.h"

BookmarksBar::BookmarksBar(const QString& settingsKeyPrefix, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookmarksBar),
    m_settingsKeyPrefix(settingsKeyPrefix)
{
    ui->setupUi(this);
    
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setColumnCount(2);
    model->setHorizontalHeaderLabels(QStringList() << "Name" << "Location");
    ui->treeBookmarks->setModel(model);
    
    // Load bookmarks
    QString bookmarksStr = SettingsLoader().get(m_settingsKeyPrefix + "bookmarks").toString();
    if (!bookmarksStr.isEmpty()) {
        QStringList allBookmarksStr = bookmarksStr.split(",");
        for (QString bookmarkStr : allBookmarksStr) {
            Bookmark bm;
            if (bm.deserialize(bookmarkStr)) {
                LOG(INFO) << "Loading bookmark [" << bm << "]";
                m_bookmarks.push_back(bm);
                
                QStandardItem* bookmarkStandardItemName = new QStandardItem(bm.name());
                QString location = bm.serialize().mid(bm.name().length() + 1 /* name= */);
                QStandardItem* bookmarkStandardItemLocation = new QStandardItem(location);
                bookmarkStandardItemName->setData(bm.serialize());
                bookmarkStandardItemName->setFlags(bookmarkStandardItemName->flags() & ~Qt::ItemIsEditable);
                bookmarkStandardItemLocation->setFlags(bookmarkStandardItemLocation->flags() & ~Qt::ItemIsEditable);
                
                int currRow = model->rowCount();
                model->setItem(currRow, 0, bookmarkStandardItemName);
                model->setItem(currRow, 1, bookmarkStandardItemLocation);
            }
        }
    }
    QObject::connect(ui->treeBookmarks, SIGNAL(doubleClicked(QModelIndex)),
                     this, SLOT(onSelectionChanged(QModelIndex)));
    
    
}

BookmarksBar::~BookmarksBar()
{
    // Save
    QString serializedForm;
    int i = 0;
    for (Bookmark bm : m_bookmarks) {
        serializedForm.append(bm.serialize());
        if (++i != m_bookmarks.size()) {
            serializedForm.append(",");
        }
    }
    SettingsLoader().saveSettings(m_settingsKeyPrefix + "bookmarks", serializedForm);
    memory::deleteAll(ui);
}

void BookmarksBar::onSelectionChanged(const QModelIndex& modelIndex)
{
    QVector<Bookmark>::Iterator bmIter = 
            std::find_if(m_bookmarks.begin(), m_bookmarks.end(), [&](const Bookmark& b) {
        QString serialized = modelIndex.sibling(modelIndex.row(), 0).data().toString() + "=" +
                modelIndex.sibling(modelIndex.row(), 1).data().toString();
        return b.serialize() == serialized;
    });
    if (bmIter != m_bookmarks.end()) {
        emit selectionChanged(bmIter);
    }
}
