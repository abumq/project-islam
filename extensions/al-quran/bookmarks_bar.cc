#include "bookmarks_bar.h"
#include "ui_bookmarksbar.h"
#include "core/settings_loader.h"
#include "core/memory.h"
#include "core/quran/chapter.h"

BookmarksBar::BookmarksBar(const QString& settingsKeyPrefix, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookmarksBar),
    m_settingsKeyPrefix(settingsKeyPrefix)
{
    ui->setupUi(this);
    
    // Load bookmarks
    QString bookmarksStr = SettingsLoader().get(m_settingsKeyPrefix + "bookmarks").toString();
    if (!bookmarksStr.isEmpty()) {
        QStringList allBookmarksStr = bookmarksStr.split(",");
        for (QString bookmarkStr : allBookmarksStr) {
            Bookmark bm;
            if (bm.deserialize(bookmarkStr)) {
                LOG(INFO) << "Loading bookmark [" << bm << "]";
                m_bookmarks.push_back(bm);
                QListWidgetItem* bookmarkItem = new QListWidgetItem(bm.name(), ui->lstBookmark);
                bookmarkItem->setToolTip(bm.serialize());
            }
        }
    }
    QObject::connect(ui->lstBookmark, SIGNAL(itemDoubleClicked(QListWidgetItem*)), 
                     this, SLOT(onSelectionChanged(QListWidgetItem*)));
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

void BookmarksBar::onSelectionChanged(QListWidgetItem* item)
{
    QVector<Bookmark>::Iterator bmIter = 
            std::find_if(m_bookmarks.begin(), m_bookmarks.end(), [&](const Bookmark& b) {
        return b.serialize() == item->toolTip();
    });
    if (bmIter != m_bookmarks.end()) {
        emit selectionChanged(bmIter);
    }
}
