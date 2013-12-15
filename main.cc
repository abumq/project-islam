#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QResource>

#include "core/logging/logging.h"
#include "core/utils/version.h"

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#   error This project requires Qt 5.x
#endif

_INITIALIZE_EASYLOGGINGPP

const int kExtraFilesCount = 1;
const char* kExtraFiles[kExtraFilesCount] = {
    ".l.lg"
};

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(styles);
    Q_INIT_RESOURCE(icons);
    Q_INIT_RESOURCE(sql_data);
    
    QApplication a(argc, argv);

    _START_EASYLOGGINGPP(argc, argv);
    
    LoggingConfigurer::configureLoggers();
    
    a.setApplicationName("Project Islam Platform");
    a.setApplicationVersion(version::versionString());
    a.setApplicationDisplayName("Project Islam Platform");

    MainWindow w(&a);
    w.show();
    
    int status = a.exec();
    
    // clean extra files
    LOG(DEBUG) << "Removing extra files";
    for (int i = 0; i < kExtraFilesCount; ++i) {
        QFile f(kExtraFiles[i]);
        if (f.exists()) {
            f.remove();
        }
    }
    
    return status;
}
