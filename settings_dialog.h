#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class ColorBox;
class MainWindow;
class QTabWidget;

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    SettingsDialog(MainWindow* mainWindow, QWidget* parent = 0);
    ~SettingsDialog();
    bool homeDirectoryChanged;
    QTabWidget* settingsTabWidget();
protected:
    void accept();
private slots:
    
    void on_rdoManualDayLightSavings_toggled(bool checked);
    
private:
    Ui::SettingsDialog *ui;
    MainWindow* m_mainWindow;
    ColorBox* m_colorBox;
    void loadSettingsInUi();
};

#endif // SETTINGS_DIALOG_H
