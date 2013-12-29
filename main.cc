#include "mainwindow.h"
#include <QSplashScreen>
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
    LoggingConfigurer::configureLoggers();
    _TRACE;
    
    Q_INIT_RESOURCE(styles);
    Q_INIT_RESOURCE(icons);
    
    QApplication a(argc, argv);
    
    _START_EASYLOGGINGPP(argc, argv);
    
    a.setApplicationName("Project Islam Platform");
    a.setOrganizationName("Project Islam");
    a.setApplicationVersion(version::versionString());
    a.setApplicationDisplayName("Project Islam Platform");
    
    QPixmap p(":/img/splash");
    QSplashScreen splashScreen(p);
    splashScreen.show();
    splashScreen.showMessage("Initializing...", Qt::AlignHCenter | Qt::AlignBottom);
    qApp->processEvents();
    
    MainWindow w(&splashScreen);
    int status;
    if (!w.applicationUpdated()) {
        w.show();
        // We cannot simply move this splashScreen.finish()
        // below this if check because we need to goto event
        // loop i.e, a.exec()
        splashScreen.finish(&w);
        status = a.exec();
    } else {
        splashScreen.finish(&w);
        status = 0;
    }
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
