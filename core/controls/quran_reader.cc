#include "quran_reader.h"
#include "ui_quran_reader.h"
#include <QResizeEvent>
#include "core/quran/quran.h"
#include "core/controls/quran_view.h"
#include "core/logging.h"
#include "core/constants.h"

QuranReader::QuranReader(quran::Quran* quran, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuranReader),
    m_quran(quran),
    m_quranView(nullptr)
{
    _TRACE;
    
    ui->setupUi(this);
    if (quran->chapters().empty()) {
        ui->cboChapter->setEnabled(false);
        ui->spnVerse->setEnabled(false);
        ui->spnVerseFrom->setEnabled(false);
        ui->spnVerseTo->setEnabled(false);
    }
    m_quranView = new QuranView(quran, this);
    m_quranView->setGeometry(5, 
                             ui->grpControls->geometry().top() + ui->grpControls->geometry().height(),
                             621, 
                             431);
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
    connect(m_quranView, SIGNAL(currentVerseChanged(int)), this, SIGNAL(currentVerseChanged(int)));
    on_btnMoreControls_clicked(false);
    
    ui->spnZoom->setValue(static_cast<double>(m_quranView->fontSize()));
}

QuranReader::~QuranReader()
{
    _TRACE;
    delete m_quranView;
    delete ui;
}

void QuranReader::changeChapter(quran::Chapter::Name chapterName)
{
    _TRACE;
    ui->cboChapter->setCurrentIndex(static_cast<int>(chapterName) - 1);
}

void QuranReader::changeVerseRange(int from, int to)
{
    _TRACE;
    if (ui->spnVerseTo->minimum() == from && ui->spnVerseFrom->maximum() == to) {
        return;
    }
    DVLOG(8) << "Changing range [" << from << " - " << to << "]";
    m_quranView->update(from, to);
    
    int currVal = ui->spnVerse->value();
    ui->spnVerse->setMinimum(from);
    ui->spnVerse->setMaximum(to);
    if (currVal != from) {
        ui->spnVerse->setValue(from);
    }
    
    ui->spnVerseTo->setMinimum(from);
    ui->spnVerseTo->setValue(to);
    ui->spnVerseFrom->setMaximum(to);
    ui->spnVerseFrom->setValue(from);
}

void QuranReader::highlightVerse(int verseNumber)
{
    _TRACE;
    ui->spnVerse->setValue(verseNumber);
    m_quranView->selectVerse(verseNumber);
}

int QuranReader::currentVerseNumber() const
{
    _TRACE;
    return m_quranView->selectedVerse() == nullptr ? -1 : m_quranView->selectedVerse()->number();
}

void QuranReader::hideChapterSelector()
{
    ui->cboChapter->hide();
    ui->lblCboChapter->hide();
}

void QuranReader::showChapterSelector()
{
    ui->cboChapter->show();
    ui->lblCboChapter->show();
}

void QuranReader::hideVerseRangeSelector()
{
    ui->spnVerseFrom->hide();
    ui->spnVerseTo->hide();
    ui->lblVerseRange->hide();
    ui->lblVerseRangeSeparator->hide();
}

void QuranReader::showVerseRangeSelector()
{
    ui->spnVerseFrom->hide();
    ui->spnVerseTo->hide();
    ui->lblVerseRange->hide();
    ui->lblVerseRangeSeparator->hide();
}

void QuranReader::hideCurrentVerseSelector()
{
    ui->spnVerse->hide();
    ui->lblVerse->hide();
}

void QuranReader::showCurrentVerseSelector()
{
    ui->spnVerse->show();
    ui->lblVerse->hide();
}

void QuranReader::on_cboChapter_currentIndexChanged(int index)
{
    _TRACE;
    int chapterId = index + 1;
    
    const quran::Chapter* chapter = m_quran->chapter(static_cast<quran::Chapter::Name>(chapterId));
    if (chapter != nullptr) {
        LOG(INFO) << "Loading chapter [" << chapter->arabicName() << "]";
        m_quranView->update(const_cast<quran::Chapter*>(chapter), 1, chapter->versesCount());
        ui->spnVerseFrom->setMinimum(1);
        ui->spnVerseFrom->setMaximum(chapter->versesCount());
        ui->spnVerseFrom->setValue(1);
        ui->spnVerseTo->setMinimum(1);
        ui->spnVerseTo->setMaximum(chapter->versesCount());
        ui->spnVerseTo->setValue(chapter->versesCount());
        ui->spnVerse->setMinimum(1);
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

void QuranReader::resizeEvent(QResizeEvent* e)
{
    if (m_quranView != nullptr) {
        m_quranView->setGeometry(5,
                                 ui->grpControls->geometry().top() + ui->grpControls->geometry().height(),
                                 e->size().width() - 5,
                                 e->size().height() - (ui->grpControls->geometry().top() + ui->grpControls->geometry().height()));
    }
    QWidget::resizeEvent(e);
}

void QuranReader::on_btnMoreControls_clicked(bool checked)
{
    _TRACE;
    ui->grpMoreControls->setVisible(checked);
    ui->grpControls->resize(ui->grpControls->width(), !checked ? 71 : 131);
    resize(width(), height() - 1);
}

void QuranReader::on_spnZoom_valueChanged(double val)
{
    m_quranView->changeSize(val);
}
