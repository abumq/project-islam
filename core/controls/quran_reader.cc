#include "quran_reader.h"
#include "ui_quran_reader.h"
#include "core/data/data_holder.h"
#include "core/logging.h"
#include "core/constants.h"
QuranReader::QuranReader(data::DataHolder* dataHolder, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuranReader),
    m_dataHolder(dataHolder),
    m_currentChapter(nullptr)
{
    ui->setupUi(this);
    // Chapters
    for (quran::SingleChapter c : m_dataHolder->quranArabic()->chapters()) {
        QString chapterItem = QString::number(static_cast<int>(c.second.name())) + ". " + 
                QString::fromStdWString(c.second.arabicName()) + 
                " (" + QString::fromStdWString(c.second.arabicScriptName()) + ")";
        ui->cboChapter->addItem(chapterItem);
    }
    ui->cboChapter->setCurrentIndex(0);
    on_cboChapter_currentIndexChanged(0);
}

QuranReader::~QuranReader()
{
    delete ui;
}

void QuranReader::update(int from, int to)
{
    update(static_cast<quran::Chapter::Name>(ui->cboChapter->currentIndex() + 1), from, to);
}

void QuranReader::update(quran::Chapter::Name chapter, int from, int to)
{
    _TRACE;
    if (static_cast<quran::Chapter::Name>(ui->cboChapter->currentIndex() + 1) != chapter) {
        m_currentChapter = const_cast<quran::Chapter*>(m_dataHolder->quranArabic()->chapter(chapter));
    }
    if (m_currentChapter == nullptr) {
        LOG(ERROR) << "Chapter index [" << static_cast<int>(chapter) << "] not found.";
        return;
    }
    LOG(INFO) << "Updating reader for chapter [" << m_currentChapter->arabicName() 
        << "] verses [" << from << " - " << to << "]";
    CHECK(from <= m_currentChapter->versesCount()) << "Chapter: [" << m_currentChapter->arabicName() <<
        "] only has [" << m_currentChapter->versesCount() << "] verses, requested verse [" << from << "]";
    CHECK(to <= m_currentChapter->versesCount()) << "Chapter: [" << m_currentChapter->arabicName() <<
        "] only has [" << m_currentChapter->versesCount() << "] verses, requested verse [" << to << "]";
    CHECK(from <= to) << "Invalid range. Verses from [" << from << "] to [" << to << "] requested.";
    
    for (int i = from; i <= to; ++i) {
        const quran::Verse* verse = &m_currentChapter->verses().at(i);
    }
}

void QuranReader::on_cboChapter_currentIndexChanged(int index)
{
    _TRACE;
    int chapterId = index + 1;
        
    const quran::Chapter* chapter = m_dataHolder->quranArabic()->chapter(static_cast<quran::Chapter::Name>(chapterId));
    if (chapter != nullptr) {
        ui->spnVerseFrom->setMaximum(chapter->versesCount());
        ui->spnVerseTo->setMaximum(chapter->versesCount());
        ui->spnVerseTo->setValue(chapter->versesCount());
        update(static_cast<quran::Chapter::Name>(chapterId), 1, chapter->versesCount());
    }
    m_currentChapter = const_cast<quran::Chapter*>(chapter);
}
