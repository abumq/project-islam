#ifndef DATABASEBUILDER_H
#define DATABASEBUILDER_H

#include <QDialog>

namespace Ui {
class DatabaseBuilder;
}

class DatabaseBuilder : public QDialog
{
    Q_OBJECT
    
public:
    explicit DatabaseBuilder(QWidget *parent = 0);
    ~DatabaseBuilder();
    
private slots:
    void on_btnBrowse_clicked();
    
    void on_btnBrowse2_clicked();
    
    void on_btnBrowse3_clicked();
    
    void on_pushButton_clicked();
    
private:
    Ui::DatabaseBuilder *ui;
};

#endif // DATABASEBUILDER_H
