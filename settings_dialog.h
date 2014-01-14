#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include <QMap>
#include <QVariant>

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
    QMap<QString, QVariant>* settingsMap();
    int exec();
protected:
    void accept();
    void reject();
private slots:
    
    void on_rdoManualDayLightSavings_toggled(bool checked);
    
private:
    Ui::SettingsDialog *ui;
    MainWindow* m_mainWindow;
    ColorBox* m_colorBox;
    void loadSettingsInUi();
    QMap<QString, QVariant> m_settingsMap;
};

#endif // SETTINGS_DIALOG_H
