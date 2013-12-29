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
    
    QString appExec = QString(argv[0]);
    QString appPath = appExec.mid(0, appExec.lastIndexOf(QDir::separator()));
    // This is important in order to load platform specific plugins
    // We do not want to use environment variables as that may be changed
    // by some other Qt application
    int argc_ = argc + 2;
#if defined(_MSC_VER)
    // We dynamically allocate because VC++ causes issue
    // TODO: Use new() instead of malloc to be compatible with delete[]
    char** argv_ = (char**)malloc(argc_ * sizeof(char*));
#else
    char* argv_[argc_];
#endif // defined(_MSC_VER)
    for (int i = 0; i < argc; ++i) {
        argv_[i] = argv[i];
    }
    argv_[argc] = "-platformpluginpath";
    argv_[argc + 1] = const_cast<char*>((appPath + "/plugins").toStdString().c_str());
    
    QApplication a(argc_, argv_);
    
    Q_INIT_RESOURCE(styles);
    Q_INIT_RESOURCE(icons);
    
    _START_EASYLOGGINGPP(argc_, argv_);
    
    LOG(INFO) << "Started app with args: " << a.arguments();
    
    a.setApplicationName("Project Islam Platform");
    a.setOrganizationName("Project Islam");
    a.setApplicationVersion(version::versionString());
    a.setApplicationDisplayName("Project Islam Platform");
    
    QPixmap p(":/img/splash");
    QSplashScreen splashScreen(p);
    splashScreen.show();
    splashScreen.showMessage("Initializing...", Qt::AlignHCenter | Qt::AlignBottom);
    qApp->processEvents();
    
    qApp->addLibraryPath(qApp->applicationDirPath());
    qApp->addLibraryPath(qApp->applicationDirPath() + "/extensions");
    
    
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
#if defined(_MSC_VER)
    delete[] argv_;
#endif // defined(_MSC_VER)
    return status;
}
