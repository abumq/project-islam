#include "quran_reader.h"
#include "ui_quran_reader.h"
#include "core/quran/quran.h"
#include "core/controls/quran_view.h"
#include "core/logging.h"
#include "core/constants.h"
QuranReader::QuranReader(quran::Quran* quran, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuranReader),
    m_quran(quran),
    m_quranView(new QuranView(quran, this))
{
    _TRACE;
    ui->setupUi(this);
    m_quranView->setGeometry(QRect(10, 40, 621, 431));
    // Chapters
    for (quran::SingleChapter c : quran->chapters()) {
        QString chapterItem = QString::number(static_cast<int>(c.second.name())) + ". " + 
                QString::fromStdWString(c.second.arabicName()) + 
                " (" + QString::fromStdWString(c.second.arabicScriptName()) + ")";
        ui->cboChapter->addItem(chapterItem);
    }
    ui->cboChapter->setCurrentIndex(0);
    on_cboChapter_currentIndexChanged(0);
    
    connect(m_quranView, SIGNAL(chapterChanged(const quran::Chapter*)), this, SIGNAL(chapterChanged(const quran::Chapter*)));
    connect(m_quranView, SIGNAL(verseRangeChanged(int,int)), this, SIGNAL(verseRangeChanged(int,int)));
    connect(m_quranView, SIGNAL(selectedVerseChanged(int)), this, SIGNAL(selectedVerseChanged(int)));
}

QuranReader::~QuranReader()
{
    delete m_quranView;
    delete ui;
}

void QuranReader::changeChapter(quran::Chapter::Name chapterName)
{
    ui->cboChapter->setCurrentIndex(static_cast<int>(chapterName) - 1);
}

void QuranReader::changeVerseRange(int from, int to)
{
    _TRACE;
    if (ui->spnVerseTo->minimum() == from && ui->spnVerseFrom->maximum() == to) {
        return;
    }
    LOG(INFO) << "Changing range [" << from << " - " << to << "]";
    m_quranView->update(from, to);
    ui->spnVerse->setMinimum(from);
    
    ui->spnVerseTo->setMinimum(from);
    ui->spnVerseTo->setValue(to);
    ui->spnVerseFrom->setMaximum(to);
    ui->spnVerseFrom->setValue(from);
}

void QuranReader::highlightVerse(int verseNumber)
{
    ui->spnVerse->setValue(verseNumber);
    m_quranView->selectVerse(verseNumber);
}

int QuranReader::currentVerse()
{
    return m_quranView->selectedVerse() == nullptr ? -1 : m_quranView->selectedVerse()->number();
}

void QuranReader::on_cboChapter_currentIndexChanged(int index)
{
    _TRACE;
    int chapterId = index + 1;
    
    const quran::Chapter* chapter = m_quran->chapter(static_cast<quran::Chapter::Name>(chapterId));
    if (chapter != nullptr) {
        LOG(INFO) << "Loading chapter [" << chapter->arabicName() << "]";
        m_quranView->update(const_cast<quran::Chapter*>(chapter), 1, chapter->versesCount());
        ui->spnVerseFrom->setMaximum(chapter->versesCount());
        ui->spnVerseFrom->setValue(1);
        ui->spnVerseTo->setMaximum(chapter->versesCount());
        ui->spnVerseTo->setValue(chapter->versesCount());
        ui->spnVerse->setMaximum(chapter->versesCount());
        ui->spnVerse->setValue(1);
    }
}

void QuranReader::on_spnVerseFrom_valueChanged(int)
{
    _TRACE;
    changeVerseRange(ui->spnVerseFrom->value(), ui->spnVerseTo->value());
}

void QuranReader::on_spnVerseTo_valueChanged(int)
{
    _TRACE;
    changeVerseRange(ui->spnVerseFrom->value(), ui->spnVerseTo->value());
}

void QuranReader::on_spnVerse_valueChanged(int)
{
    _TRACE;
    if (m_quranView->selectedVerse() != nullptr 
        && m_quranView->selectedVerse()->number() != ui->spnVerse->value()) {
        m_quranView->selectVerse(ui->spnVerse->value());
    }
}
