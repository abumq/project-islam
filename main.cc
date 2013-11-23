#include "mainwindow.h"
#include <QApplication>
#include <QResource>
#include "core/logging.h"

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(styles);
    Q_INIT_RESOURCE(icons);
    Q_INIT_RESOURCE(sql_data);
    
    QApplication a(argc, argv);

    _START_EASYLOGGINGPP(argc, argv);
    
    configureLoggers();
    
    QApplication::setApplicationName("Project Islam Platform");
    QApplication::setApplicationDisplayName("Project Islam Platform");
    QApplication::setApplicationVersion("v0.1");
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
