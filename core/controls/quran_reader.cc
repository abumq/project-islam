#include "quran_reader.h"
#include "ui_quran_reader.h"

#include <QResizeEvent>
#include <QMenu>

#include "core/logging/logging.h"
#include "core/utils/memory.h"
#include "core/quran/quran.h"
#include "core/controls/quran_view.h"
#include "core/constants.h"

QuranReader::QuranReader(quran::Quran* quran, quran::Quran* quranTranslation, 
                         quran::Quran* quranTransliteration,
                         quran::Quran* quranTafseer, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuranReader),
    m_quran(quran),
    m_quranTranslation(quranTranslation),
    m_quranTransliteration(quranTransliteration),
    m_quranTafseer(quranTafseer),
    m_quranView(nullptr)
{
    _TRACE;
    
    ui->setupUi(this);
    if (m_quran != nullptr) {
        if (m_quran->chapters().size() != quran::Quran::kChapterCount) {
            ui->cboChapter->setEnabled(false);
            ui->spnVerse->setEnabled(false);
            ui->spnVerseFrom->setEnabled(false);
            ui->spnVerseTo->setEnabled(false);
        }
        m_quranView = new QuranView(m_quran, m_quranTranslation, m_quranTransliteration, m_quranTafseer, this);
        m_quranView->setObjectName("QuranView");
        m_quranView->setGeometry(5, 
                                 ui->grpControls->geometry().top() + ui->grpControls->geometry().height(),
                                 621, 
                                 431);
        // Chapters
        for (quran::SingleChapter c : m_quran->chapters()) {
            QString chapterItem = QString::number(static_cast<int>(c.second.name())) + ". " + 
                    QString::fromStdWString(c.second.arabicName()) + 
                    " (" + QString::fromStdWString(c.second.arabicScriptName()) + ")";
            ui->cboChapter->addItem(chapterItem);
        }
        ui->cboChapter->setCurrentIndex(0);
        
        connect(m_quranView, SIGNAL(chapterChanged(const quran::Chapter*)), this, SIGNAL(chapterChanged(const quran::Chapter*)));
        connect(m_quranView, SIGNAL(verseRangeChanged(int,int)), this, SIGNAL(verseRangeChanged(int,int)));
        connect(m_quranView, SIGNAL(currentVerseChanged(int)), this, SIGNAL(currentVerseChanged(int)));
        connect(m_quranView, SIGNAL(translationToggled(bool)), this, SIGNAL(translationToggled(bool)));
        connect(m_quranView, SIGNAL(transliterationOnToggled(bool)), this, SIGNAL(transliterationOnToggled(bool)));
        connect(m_quranView, SIGNAL(tafseerToggled(bool)), this, SIGNAL(tafseerToggled(bool)));
        connect(ui->txtJumpTo, SIGNAL(textChanged(QString)), this, SIGNAL(jumpToTextChanged(QString)));
        on_btnMoreControls_clicked(false);
    }
}

QuranReader::~QuranReader()
{
    _TRACE;
    memory::deleteAll(m_quranView, ui);
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
    if (currVal < from || currVal > to) {
        ui->spnVerse->setValue(from);
    }
    
    ui->spnVerseTo->setMinimum(from);
    ui->spnVerseTo->setValue(to);
    ui->spnVerseFrom->setMaximum(to);
    ui->spnVerseFrom->setValue(from);
    
    ui->txtJumpTo->setText(m_quranView->jumpToText());
}

void QuranReader::highlightVerse(int verseNumber)
{
    _TRACE;
    ui->spnVerse->setValue(verseNumber);
    m_quranView->selectVerse(verseNumber);
    refreshJumpToText();
}

void QuranReader::refreshJumpToText() {
    _TRACE;
    ui->txtJumpTo->setText(m_quranView->jumpToText());
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
    ui->spnVerseFrom->show();
    ui->spnVerseTo->show();
    ui->lblVerseRange->show();
    ui->lblVerseRangeSeparator->show();
}

void QuranReader::hideCurrentVerseSelector()
{
    ui->spnVerse->hide();
    ui->lblVerse->hide();
}

void QuranReader::showCurrentVerseSelector()
{
    ui->spnVerse->show();
    ui->lblVerse->show();
}

void QuranReader::turnOnTranslation()
{
    _TRACE;
    ui->chkTranslation->setChecked(true);
    m_quranView->turnOnTranslation(m_quranTranslation);
}

void QuranReader::turnOffTranslation()
{
    _TRACE;
    ui->chkTranslation->setChecked(false);
    m_quranView->turnOffTranslation();
}


void QuranReader::turnOnTafseer()
{
    _TRACE;
    ui->chkTafseer->setChecked(true);
    m_quranView->turnOnTafseer(m_quranTafseer);
}

void QuranReader::turnOffTafseer()
{
    _TRACE;
    ui->chkTafseer->setChecked(false);
    m_quranView->turnOffTafseer();
}

void QuranReader::turnOffTransliteration()
{
    _TRACE;
    ui->chkTransliteration->setChecked(false);
    m_quranView->turnOffTransliteration();
}

void QuranReader::turnOnTransliteration()
{
    _TRACE;
    ui->chkTransliteration->setChecked(true);
    m_quranView->turnOnTransliteration(m_quranTransliteration);
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
        ui->txtJumpTo->setText(m_quranView->jumpToText());
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
    ui->grpControls->resize(ui->grpControls->width(), !checked ? 71 : 161);
    resize(width(), height() - 1);
    ui->btnMoreControls->setToolTip(QString(checked ? "Hide" : "Show") + " more controls...");
}

void QuranReader::on_btnZoomOut_clicked()
{
    m_quranView->zoomOut();
}

void QuranReader::on_btnZoomIn_clicked()
{
    
    m_quranView->zoomIn();
}

void QuranReader::on_chkTransliteration_clicked(bool checked)
{
    if (checked) {
        turnOnTransliteration();
    } else {
        turnOffTransliteration();
    }
}

void QuranReader::on_chkTranslation_clicked(bool checked)
{
    if (checked) {
        turnOnTranslation();
    } else {
        turnOffTranslation();
    }
    
}

void QuranReader::on_txtJumpTo_returnPressed()
{
    _TRACE;
    if (m_quranView != nullptr) {
        m_quranView->jumpTo(ui->txtJumpTo->text());
    }
}

void QuranReader::on_chkTafseer_clicked(bool checked)
{
    if (checked) {
        turnOnTafseer();
    } else {
        turnOffTafseer();
    }
}
