#include "extensions/memorization.h"

#include <QResizeEvent>
#include <QTableView>
#include <QStandardItemModel>

#include "core/controls/quran_reciter.h"
#include "core/controls/quran_reader.h"

Memorization::Memorization(QWidget *parent, data::DataHolder* dataHolder) :
    Extension(parent, dataHolder, "Memorization", "Quran Memorization",
        "Memorize glorious Quran with help of this extension. " \
        "This extension helps you keep track of your progress " \
        "of Quran memorization."
    ),
    m_reciter(nullptr),
    m_reader(nullptr),
    m_tableView(nullptr)
{
    initialize();
}

Memorization::~Memorization()
{
    _TRACE;
    delete m_tableView;
    m_tableView = nullptr;
}

void Memorization::initialize()
{
    _TRACE;
    m_reciter = new QuranReciter(dataHolder(), container());
    m_reader = new QuranReader(dataHolder(), container());
    m_tableView = new QTableView(container());
    m_tableView->hide();
}

void Memorization::resizeEvent(QResizeEvent* e)
{
    Extension::resizeEvent(e);
    if (m_reciter != nullptr) {
        int centerW = (container()->width() / 2) - (m_reciter->width() / 2);
        int bottom = container()->height() - m_reciter->height() - 100;
        m_reciter->move(centerW, bottom);
    }
}
