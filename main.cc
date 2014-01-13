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

int returnCode;

class CleanAtEnd {
public:
    CleanAtEnd() {
    }

    ~CleanAtEnd() {
        // clean extra files
        DVLOG(9) << "Removing extra files";
        for (int i = 0; i < kExtraFilesCount; ++i) {
            QFile f(kExtraFiles[i]);
            if (f.exists()) {
                f.remove();
            }
        }
        DVLOG(9) << "Exit status: " << returnCode;
    }
    
private:
    static const int kExtraFilesCount = 1;
    const char* kExtraFiles[kExtraFilesCount] = {
        ".l.lg"
    };
};

int main(int argc, char* argv[])
{
    CleanAtEnd cleaner;
    Q_UNUSED(cleaner);
    
    LoggingConfigurer::configureLoggers();
    _TRACE;
    
#if defined(Q_OS_WIN)
    QString appExec = QString(argv[0]);
    QString pluginsPath = appExec.mid(0, appExec.lastIndexOf(QDir::separator())) + QDir::separator()
            + "plugins";
    // This is important in order to load platform specific plugins
    // We do not want to use environment variables as that may be changed
    // by some other Qt application
    int argc_ = argc + 2;
    // We dynamically allocate because VC++ causes issue
    const char** argv_ = (const char**)malloc(argc_ * sizeof(char*));
    int i = 0;
    for (; i < argc; ++i) {
        argv_[i] = argv[i];
    }
#   if (!defined(_DEBUG))
    // We only change argv in non-debug mode to prevent ASSERT(argc == original_argc) failure
    argv_[i++] = "-platformpluginpath";
    std::string pluginsPathStr = pluginsPath.toStdString();
    argv_[i++] = pluginsPathStr.c_str();
#   endif // (!defined(_DEBUG))
#else
    int argc_ = argc;
    char** argv_ = argv;
#endif // defined(Q_OS_WIN)
    // NOTE: As per doc says, qApp->arguments() would be parsed
    // from main(argc, argv) instead of argc_ and argv_ so qApp->arguments() may not
    // behave as expected.
    // See http://qt-project.org/doc/qt-5.0/qtcore/qcoreapplication.html#arguments
    QApplication a(argc_, const_cast<char**>(argv_));
    
    Q_INIT_RESOURCE(styles);
    Q_INIT_RESOURCE(icons);
    
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
    if (!w.applicationUpdated()) {
        w.show();
        // We cannot simply move this splashScreen.finish()
        // below this if check because we need to goto event
        // loop i.e, a.exec()
        return a.exec();
    } else {
        returnCode = 0;
    }
#if defined(Q_OS_WIN)
    free(argv_);
#endif // defined(Q_OS_WIN)
    return returnCode;
}
