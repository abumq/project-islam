#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class ColorBox;
class MainWindow;

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(MainWindow* mainWindow, QWidget* parent = 0);
    ~SettingsDialog();
protected:
    void accept();
private slots:
    void on_chkLevelGlobal_clicked(bool checked);
    
    void on_buildQuranEnglish_clicked();
    
    void on_buildQuranArabic_clicked();
    
    void on_buildQuranChapters_clicked();
    
    void on_buildDua_clicked();
    
private:
    Ui::SettingsDialog *ui;
    MainWindow* m_mainWindow;
    ColorBox* m_colorBox;
    void loadSettingsInUi();
    
};

#endif // SETTINGS_DIALOG_H
