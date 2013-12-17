#include "databasebuilder.h"
#include "ui_databasebuilder.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <easylogging++.h>
#include "dataconverter.h"
#include "databuilder.h"

DatabaseBuilder::DatabaseBuilder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseBuilder)
{
    ui->setupUi(this);
    on_btnReset_clicked();
    on_btnReset_2_clicked();
}

DatabaseBuilder::~DatabaseBuilder()
{
    delete ui;
}

void DatabaseBuilder::on_btnBrowse_clicked()
{
    QFileDialog d;
    ui->txtDataFile->setText(d.getOpenFileName(this, "Browse Quran Data File", ui->txtDataFile->text()));
    QFileInfo fi(ui->txtDataFile->text());
    ui->txtTableName->setText("Quran__" + fi.fileName());
}

void DatabaseBuilder::on_btnBrowse2_clicked()
{
    QFileDialog d;
    ui->txtRukuhSajdahFile->setText(d.getOpenFileName(this, "Browse Base Info File", ui->txtRukuhSajdahFile->text()));
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
    if (m_convertDataList.empty() || 
        (!ui->txtTableName->text().isEmpty() 
         && std::find(m_convertDataList.begin(), m_convertDataList.end(), ui->txtTableName->text().toStdString()) 
         == m_convertDataList.end())) {
        LOG(WARNING) << "Forcing to add table [" << ui->txtTableName->text() << "]";
        on_btnAdd_2_clicked();
    }
    if (m_convertDataList.empty()) {
        QMessageBox::warning(this, tr("Invalid data"), "No data found to convert - Please valid select data first!");
        return;
    }
    ui->txtDataFile->setEnabled(false);
    ui->txtRukuhSajdahFile->setEnabled(false);
    ui->txtTableName->setEnabled(false);
    ui->txtOutputFile->setEnabled(false);
    DataConverter dc;
    for (ConvertData cd : m_convertDataList) {
        dc.startConvert(&cd);
    }
    ui->txtDataFile->setEnabled(true);
    ui->txtRukuhSajdahFile->setEnabled(true);
    ui->txtTableName->setEnabled(true);
    ui->txtOutputFile->setEnabled(true);
    QMessageBox::information(this, tr("Success"), "Successfully converted!");
}

void DatabaseBuilder::on_btnBrowse4_clicked()
{
    QFileDialog d;
    ui->txtSqliteFilename->setText(d.getOpenFileName(this, "Browse SQLite 3 File", ui->txtSqliteFilename->text()));
}

void DatabaseBuilder::on_btnBrowse5_clicked()
{
    QFileDialog d;
    QStringList s = d.getOpenFileNames(this, "Browse SQL File(s)", ui->txtSqFilename->text());
    if (s.size() > 1) {
        for (QString f : s) {
            m_sqlFiles.append(f);
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeSqlFiles);
            item->setText(0, f);
        }
    } else if (s.size() == 1) {
        ui->txtSqFilename->setText(s.at(0));
    }
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
    QMessageBox::information(this, tr("Success"), "Successfully built!");
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

void DatabaseBuilder::on_pushButton_3_clicked()
{
    m_convertDataList.clear();
    while (ui->treeWidget->topLevelItemCount() > 0)
        ui->treeWidget->takeTopLevelItem(0); 
}

void DatabaseBuilder::on_btnAdd_2_clicked()
{
    ConvertData cd(ui->txtRukuhSajdahFile->text().toStdString(), ui->txtTableName->text().toStdString(),
                   ui->txtDataFile->text().toStdString(), ui->txtOutputFile->text().toStdString());
    m_convertDataList.push_back(cd);
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0, QString(cd.tableName.c_str()));
}
