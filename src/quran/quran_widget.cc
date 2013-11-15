#include "quran/quran_widget.hh"
#include "quran/quran_reader.hh"
#include "ui_quran_widget.h"
#include "log/logging.hh"
#include <QMainWindow>
#include <QCheckBox>
#include <QToolBar>

namespace quran {
namespace ui {

QuranWidget::QuranWidget(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::QuranWidget)
{
    ui->setupUi(this);
    
    toolBar = new QToolBar(this);
    toolBar->setObjectName(QStringLiteral("toolBar"));
    static_cast<QMainWindow*>(parent)->addToolBar(Qt::TopToolBarArea, toolBar);
    QCheckBox* chkSearch = new QCheckBox("Search", toolBar);
    QObject::connect(chkSearch, SIGNAL(clicked(bool)), this, SLOT(toggleSearch(bool)));
    toolBar->addWidget(chkSearch);
    toggleSearch(false);
    
    reader = new quran::ui::QuranReader();
    ui->versesViewer->setDocument(reader);
    
    for (std::size_t i = 1; i <= quran.chapters().size(); ++i) {
        const quran::Chapter* chapter = CHECK_NOTNULL(quran.getChapter(i));
        ui->cboChapter->addItem(
            QString::number(static_cast<int>(chapter->name()))
            + ". " + QString::fromStdWString(chapter->arabicName())
            + " (" + QString::fromStdWString(chapter->arabicScriptName()) + ")"
        );
    }
    ui->versesViewer->document()->setUndoRedoEnabled(true);
    selectedVerse = false;
}

QuranWidget::~QuranWidget()
{
    delete ui;
}

void QuranWidget::on_cboChapter_currentIndexChanged(int index)
{
    ui->versesViewer->clear();
    int selectedChapter = index + 1;
    chapter = quran.getChapter(selectedChapter);
    
    QString chapterInfo;
    chapterInfo.append("Verses: <b>" + QString::number(chapter->versesCount()) + "</b>&nbsp;&nbsp;&nbsp;");
    chapterInfo.append("Rukuh: <b>" + QString::number(chapter->rukuhCount()) + "</b>&nbsp;&nbsp;&nbsp;");
    chapterInfo.append("Sajdah: <b>" + QString::number(chapter->sajdahCount()) + "</b>&nbsp;&nbsp;&nbsp;");
    chapterInfo.append("Classification: <b>" + (chapter->classification() == quran::Chapter::Classification::Meccan ? QString("Meccan") : QString("Medinan")) + "</b><br/>");
    chapterInfo.append("Juz: <b>");
    for (std::size_t i = 0; i < chapter->positions().size(); ++i) {
        chapterInfo.append(QString::number(chapter->positions().at(i)));
        if (i < chapter->positions().size() - 1) {
            chapterInfo.append(", ");
        }
    }
    chapterInfo.append("</b>&nbsp;&nbsp;&nbsp;");
    if (!chapter->muqattaat().empty()) {
        chapterInfo.append("Muqattaat: <b>" + QString::fromStdWString(chapter->muqattaat()) + "</b>");
    }
    ui->chapterInfo->setText(chapterInfo);
    ui->spnVerse->setMaximum(chapter->versesCount());
    ui->spnVerse->setValue(1);
    reader->update(chapter);
    selectedVerse = false;
}

void QuranWidget::on_spnVerse_valueChanged(int verseNumber)
{
    ui->versesViewer->scrollToAnchor(reader->highlightVerse(verseNumber));
}

void QuranWidget::on_btnSearch_clicked()
{
    ui->versesViewer->scrollToAnchor(reader->find(ui->txtSearch->text()));
}

void QuranWidget::toggleSearch(bool val)
{
    ui->btnSearch->setVisible(val);
    ui->txtSearch->setVisible(val);
}


} // namespace ui
} // namespace quran
