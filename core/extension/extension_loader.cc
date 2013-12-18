#include "core/extension/extension_loader.h"

#include <QPluginLoader>
#include <QFileDialog>
#include <QAction>
#include <QApplication>
#include <QMenuBar>
#include <QSplashScreen>

#include "core/logging/logging.h"
#include "core/extension/extension_base.h"
#include "core/extension/abstract_extension.h"
#include "core/extension/extension_bar.h"
#include "core/data/data_holder.h"

ExtensionLoader::ExtensionLoader(data::DataHolder* dataHolder, 
                                 SettingsLoader* settingsLoader, QApplication* app, QMenuBar* menuBar) :
    m_dataHolder(dataHolder),
    m_settingsLoader(settingsLoader),
    m_app(app),
    m_menuBar(menuBar)
{
}

void ExtensionLoader::loadAll(const QString& appPath, ExtensionBar* extensionBar, QSplashScreen *splashScreen) const
{
    _TRACE;
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
#endif
    QDir extensionsDir(appPath + "/extensions/", libExtension, QDir::Name | QDir::IgnoreCase, QDir::Files);
    QStringList list = extensionsDir.entryList();
    int count = 0;
    for (QString extensionFilename : list) {
        ++count;
        QString extensionDisplayName = extensionFilename;
        if (extensionDisplayName.startsWith("lib")) {
            extensionDisplayName = extensionDisplayName.remove(0, QString("lib").length());
        }
        extensionDisplayName = extensionDisplayName.mid(0, extensionDisplayName.length() - 
                                                        libExtension.length() + 1 /* wildcard */);
        if (splashScreen != nullptr) {
            splashScreen->showMessage("Loading Extensions (" + QString::number(count) 
                                      + " / " + QString::number(list.size()) + ") [" 
                                      + extensionDisplayName + "]...", Qt::AlignHCenter | Qt::AlignBottom);
        }
        m_app->processEvents();
        QPluginLoader loader(extensionsDir.absoluteFilePath(extensionFilename));
        ExtensionBase* extensionBase = qobject_cast<ExtensionBase*>(loader.instance());
        if (extensionBase != nullptr && extensionBase->extension() != nullptr) {
            extensionBase->extension()->setDataHolder(m_dataHolder);
            extensionBase->extension()->setContainer(extensionBar->container());
            extensionBase->extension()->setSettingsLoader(m_settingsLoader);
            
            QAction* helpMenu = m_menuBar->actions().at(m_menuBar->actions().size() - 1);
            m_menuBar->insertMenu(helpMenu, extensionBase->extension()->menu());
            // Extensions may change the configurations so we reconfigure them
            LoggingConfigurer::configureLoggers();
            // Arguments
            QStringList arguments = m_app->arguments();
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
            // initialize and add to extension bar
            extensionBase->initialize(argc, argv);
#if defined(_MSC_VER)
            for(int i = 0; i < argc; ++i)
                delete[] argv[i];
#endif // defined(_MSC_VER)
            extensionBar->addExtension(extensionBase->extension());
        } else {
            LOG(ERROR) << "Error occured while loading extension [" << loader.fileName() << "]: " << loader.errorString();
        }
    }
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
