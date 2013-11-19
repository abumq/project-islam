#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QDockWidget;
class QToolBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *);
    
private:
    Ui::MainWindow *ui;
    QDockWidget* m_sidebar;
    QDockWidget* m_mainArea;
    QToolBar* m_toolbar;
    
    void setup(void);
    
    void lockSidebar(void);
    void unlockSidebar(void);
    
private slots:
    void toggleSidebar(bool);
};

#endif // MAINWINDOW_H
