#include "databasebuilder.h"
#include "ui_databasebuilder.h"
#include <QFileDialog>

DatabaseBuilder::DatabaseBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseBuilder)
{
    ui->setupUi(this);
}

DatabaseBuilder::~DatabaseBuilder()
{
    delete ui;
}

void DatabaseBuilder::on_btnBrowse_clicked()
{
    QFileDialog d;
    ui->txtDataFile->setText(d.getOpenFileName(this, "Browse Quran Data File"));
}

void DatabaseBuilder::on_btnBrowse2_clicked()
{
    QFileDialog d;
    ui->txtRukuhSajdahFile->setText(d.getOpenFileName(this, "Browse Rukuh / Sajdah File"));
}
