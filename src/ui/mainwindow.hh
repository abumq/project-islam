#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QMap>
class QDockWidget;
namespace Ui {
class MainWindow;
}
namespace ui {
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow(void);
    void lockSidebar(void);
    void unlockSidebar(void);
private:
    Ui::MainWindow* ui;
    QDockWidget* sidebar;
    QMap<QString, QWidget*> m_features;
    void buildDockedWidgets(void);
};
}
#endif // MAINWINDOW_HH
