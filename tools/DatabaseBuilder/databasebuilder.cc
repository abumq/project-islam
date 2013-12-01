#include "databasebuilder.h"
#include "ui_databasebuilder.h"
#include <QFileDialog>
#include <QMessageBox>

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

void DatabaseBuilder::on_btnBrowse3_clicked()
{
    QFileDialog d;
    ui->txtOutputFile->setText(d.getSaveFileName(this, "Output File", QString(), "*.sql"));
    if (!ui->txtOutputFile->text().endsWith(".sql")) {
        ui->txtOutputFile->setText(ui->txtOutputFile->text().append(".sql"));
    }
}

void DatabaseBuilder::on_pushButton_clicked()
{
    if (ui->txtDataFile->text().isEmpty() || ui->txtRukuhSajdahFile->text().isEmpty() || 
        ui->txtRukuhSajdahFile->text().isEmpty() || ui->txtTableName->text().isEmpty()) {
        QMessageBox::critical(this, tr("Field required"), tr("All fields are required"));
        return;
    }
    ui->txtDataFile->setEnabled(false);
    ui->txtRukuhSajdahFile->setEnabled(false);
    ui->txtTableName->setEnabled(false);
    ui->txtOutputFile->setEnabled(false);
    
    
    
    ui->txtDataFile->setEnabled(true);
    ui->txtRukuhSajdahFile->setEnabled(true);
    ui->txtTableName->setEnabled(true);
    ui->txtOutputFile->setEnabled(true);
    
}
