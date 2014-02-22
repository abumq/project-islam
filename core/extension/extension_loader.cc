#include "core/extension/extension_loader.h"

#include <QPluginLoader>
#include <QFileDialog>
#include <QAction>
#include <QApplication>
#include <QMenuBar>
#include <QTabWidget>
#include <QSplashScreen>

#include "core/logging/logging.h"
#include "core/extension/extension_base.h"
#include "core/extension/abstract_extension.h"
#include "core/extension/extension_bar.h"
#include "core/data/data_holder.h"
#include "settings_dialog.h"

ExtensionLoader::ExtensionLoader(data::DataHolder* dataHolder, QMenuBar* menuBar, SettingsDialog *settingsDialog, QSystemTrayIcon *trayIcon) :
    m_dataHolder(dataHolder),
    m_menuBar(menuBar),
    m_settingsDialog(settingsDialog),
    m_trayIcon(trayIcon)
{
}

void ExtensionLoader::loadAll(ExtensionBar* extensionBar, QSplashScreen *splashScreen) const
{
    _TRACE;
    QString appPath = qApp->applicationDirPath();
    LOG(INFO) << "Loading all the extensions. ExtensionBar [" << extensionBar << "]; application path: "
              << appPath;
    QString libExtension;
#if defined(Q_OS_WIN)
    libExtension = "*.dll";
#elif defined(Q_OS_LINUX)
    libExtension = "*.so";
#elif defined(Q_OS_MAC)
    libExtension = "*.dylib";
#else
#   error Invalid OS detected
#endif // defined(Q_OS_WIN)
    QDir extensionsDir(appPath + "/extensions/", libExtension, QDir::Name | QDir::IgnoreCase, QDir::Files);
    if (!extensionsDir.exists()) {
        return;
    }
    QStringList list = extensionsDir.entryList();
    if (list.empty()) {
        return;
    }
    // Arguments
    QStringList arguments = qApp->arguments();
    const int argc = arguments.size();
#if defined(_MSC_VER)
    // We dynamically allocate because VC++ causes issue
    const char** argv = (const char**)malloc(argc * sizeof(char*));
#else
    const char* argv[argc];
#endif // defined(_MSC_VER)
    for (int i = 0; i < argc; ++i) {
        argv[i] = arguments.at(i).toStdString().c_str();
    }
    int numberOfExtensions = 0;
    for (QString extensionFilename : list) {
        ++numberOfExtensions;
        QString extensionDisplayName = extensionFilename;
        if (extensionDisplayName.startsWith("lib")) {
            extensionDisplayName = extensionDisplayName.remove(0, QString("lib").length());
        }
        extensionDisplayName = extensionDisplayName.mid(0, extensionDisplayName.length() - 
                                                        libExtension.length() + 1 /* wildcard */);
        if (splashScreen != nullptr) {
            splashScreen->showMessage("Loading Extensions (" + QString::number(numberOfExtensions) 
                                      + " / " + QString::number(list.size()) + ") [" 
                                      + extensionDisplayName + "]...", Qt::AlignHCenter | Qt::AlignBottom);
            qApp->processEvents();
        }
        QPluginLoader loader(extensionsDir.absoluteFilePath(extensionFilename));
        ExtensionBase* extensionBase = qobject_cast<ExtensionBase*>(loader.instance());
        if (extensionBase != nullptr && extensionBase->extension() != nullptr) {
            extensionBase->extension()->setDataHolder(m_dataHolder);
            extensionBase->extension()->setContainer(extensionBar->container());
            extensionBase->extension()->setSettingsLoader(SettingsLoader::getInstance());
            extensionBase->extension()->setSettingsMap(m_settingsDialog->settingsMap());
            extensionBase->extension()->setTrayIcon(m_trayIcon);
            el::base::RegisteredLoggers* r = ELPP->registeredLoggers();
            
            QAction* helpMenu = m_menuBar->actions().at(m_menuBar->actions().size() - 1);
            m_menuBar->insertMenu(helpMenu, extensionBase->extension()->menu());
            // Extensions may change the configurations so we reconfigure them
            LoggingConfigurer::configureLoggers();
            // initialize and add to extension bar
            extensionBase->setLoggingRepository(ELPP);
            extensionBase->initialize(argc, argv);
            extensionBar->addExtension(extensionBase->extension());
            if (extensionBase->extension()->settingsTabWidget() != nullptr) {
                m_settingsDialog->settingsTabWidget()->addTab(extensionBase->extension()->settingsTabWidget(), 
                    extensionBase->extensionInfo().name());
            }
        } else {
            LOG(ERROR) << "Error occured while loading extension [" << loader.fileName() << "]: " << loader.errorString();
        }
    }
#if defined(_MSC_VER)
    free(argv);
#endif // defined(_MSC_VER)
}

bool ExtensionLoader::verifyExtension(const QString &filename)
{    
    QPluginLoader loader(filename);
    if (qobject_cast<ExtensionBase*>(loader.instance()) != nullptr) {
        return true;
    }
    LOG(ERROR) << "Error occured while verifying extension [" << loader.fileName() << "]: " 
               << loader.errorString();
    return false;
}
