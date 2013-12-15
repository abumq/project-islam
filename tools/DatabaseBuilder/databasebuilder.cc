#include "databasebuilder.h"
#include "ui_databasebuilder.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include "dataconverter.h"
#include "databuilder.h"

DatabaseBuilder::DatabaseBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseBuilder)
{
    ui->setupUi(this);
    on_btnReset_clicked();
    on_btnReset_clicked();
}

DatabaseBuilder::~DatabaseBuilder()
{
    delete ui;
}

void DatabaseBuilder::on_btnBrowse_clicked()
{
    QFileDialog d;
    ui->txtDataFile->setText(d.getOpenFileName(this, "Browse Quran Data File", ui->txtDataFile->text()));
}

void DatabaseBuilder::on_btnBrowse2_clicked()
{
    QFileDialog d;
    ui->txtRukuhSajdahFile->setText(d.getOpenFileName(this, "Browse Rukuh / Sajdah File", ui->txtRukuhSajdahFile->text()));
}

void DatabaseBuilder::on_btnBrowse3_clicked()
{
    QFileDialog d;
    ui->txtOutputFile->setText(d.getSaveFileName(this, "Output File", ui->txtOutputFile->text(), "*.sql"));
    if (!ui->txtOutputFile->text().endsWith(".sql")) {
        ui->txtOutputFile->setText(ui->txtOutputFile->text().append(".sql"));
    }
}

void DatabaseBuilder::on_pushButton_clicked()
{
    if (ui->txtDataFile->text().isEmpty() || ui->txtRukuhSajdahFile->text().isEmpty() || 
            ui->txtTableName->text().isEmpty() || ui->txtOutputFile->text().isEmpty()) {
        QMessageBox::critical(this, tr("Field required"), tr("All fields are required"));
        return;
    }
    ui->txtDataFile->setEnabled(false);
    ui->txtRukuhSajdahFile->setEnabled(false);
    ui->txtTableName->setEnabled(false);
    ui->txtOutputFile->setEnabled(false);
    
    DataConverter dc;
    
    dc.startConvert(ui->txtRukuhSajdahFile->text().toStdString(),
                    ui->txtTableName->text().toStdString(), ui->txtDataFile->text().toStdString(),
                    ui->txtOutputFile->text().toStdString());
    
    ui->txtDataFile->setEnabled(true);
    ui->txtRukuhSajdahFile->setEnabled(true);
    ui->txtTableName->setEnabled(true);
    ui->txtOutputFile->setEnabled(true);
    QMessageBox msg;
    msg.setText("Successfully converted!");
    msg.exec();
}

void DatabaseBuilder::on_btnBrowse4_clicked()
{
    QFileDialog d;
    ui->txtSqliteFilename->setText(d.getOpenFileName(this, "Browse SQLite 3 File", ui->txtSqliteFilename->text()));
}

void DatabaseBuilder::on_btnBrowse5_clicked()
{
    QFileDialog d;
    ui->txtSqFilename->setText(d.getOpenFileName(this, "Browse SQL File", ui->txtSqFilename->text()));
}

void DatabaseBuilder::on_btnBuild_clicked()
{
    if (ui->txtSqFilename->text().isEmpty() || 
            ui->txtSqliteFilename->text().isEmpty()) {
        QMessageBox::critical(this, tr("Field required"), tr("All fields are required"));
        return;
    }
    ui->txtSqFilename->setEnabled(false);
    ui->txtSqliteFilename->setEnabled(false);
    
    DataBuilder db(ui->txtSqFilename->text().toStdString(),
                   ui->txtSqliteFilename->text().toStdString());
    
    db.build(m_sqlFiles);
    ui->txtSqFilename->setEnabled(true);
    ui->txtSqliteFilename->setEnabled(true);
    QMessageBox msg;
    msg.setText("Successfully built!");
    msg.exec();
}

void DatabaseBuilder::on_btnReset_clicked()
{
    ui->txtDataFile->setText("");
    ui->txtTableName->setText("Quran__English_Translation_");
    ui->txtOutputFile->setText("");
}

void DatabaseBuilder::on_btnReset_2_clicked()
{
    ui->txtSqFilename->setText("");
}

void DatabaseBuilder::on_btnAdd_clicked()
{
    m_sqlFiles.append(ui->txtSqFilename->text());
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeSqlFiles);
    item->setText(0, ui->txtSqFilename->text());
}

void DatabaseBuilder::on_pushButton_2_clicked()
{
    m_sqlFiles.clear();
    while (ui->treeSqlFiles->topLevelItemCount() > 0)
        ui->treeSqlFiles->takeTopLevelItem(0); 
}
