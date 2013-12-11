#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "core/style_loader.h"
#include "core/settings_loader.h"
#include "core/data/data_holder.h"

#include "core/update/update_manager.h"

namespace Ui {
class MainWindow;
}

class AbstractExtension;
class ExtensionBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QApplication* app);
    ~MainWindow();

    void initialize();
    void reloadStyles();
    void loadSettings();
    SettingsLoader *settingsLoader();
    StyleLoader *styleLoader();
    
    data::DataHolder* dataHolder();
protected:
    void resizeEvent(QResizeEvent *);
private slots:
    void on_actionAbout_Qt_triggered();
    
    void on_actionExit_triggered();
    void onExtensionChanged(AbstractExtension* extension);
    void on_action_Settings_triggered();
    
    void on_actionFull_Screen_triggered(bool checked);
    void restart();
    
private:
    Ui::MainWindow *ui;
    QApplication* m_app;
    QWidget* m_container;
    ExtensionBar* m_extensionBar;
    StyleLoader m_styleLoader;
    SettingsLoader m_settingsLoader;
    Qt::WindowStates m_previousWindowsState;
    data::DataHolder m_dataHolder;
    
    UpdateManager m_updateManager;
};

#endif // MAINWINDOW_H
