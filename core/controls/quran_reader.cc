#include "quran_reader.h"
#include "ui_quran_reader.h"
#include "core/data/data_holder.h"
#include "core/logging.h"
#include "core/constants.h"
QuranReader::QuranReader(data::DataHolder* dataHolder, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuranReader),
    m_dataHolder(dataHolder)
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
    
}

void QuranReader::on_cboChapter_currentIndexChanged(int index)
{
    int chapterId = index + 1;
        
    const quran::Chapter* chapter = m_dataHolder->quranArabic()->chapter(static_cast<quran::Chapter::Name>(chapterId));
    ui->spnVerseFrom->setMaximum(chapter->versesCount());
    ui->spnVerseTo->setMaximum(chapter->versesCount());
    ui->spnVerseTo->setValue(chapter->versesCount());
    update(static_cast<quran::Chapter::Name>(chapterId), 1, chapter->versesCount());
}
