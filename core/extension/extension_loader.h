#ifndef EXTENSION_LOADER_H
#define EXTENSION_LOADER_H

#include <QString>
#include "core/logging/easylogging++.h"

class ExtensionBar;
class SettingsLoader;
class QMenuBar;
class SettingsDialog;
class QSplashScreen;
class QSystemTrayIcon;
namespace data {
class DataHolder;
}

class ExtensionLoader
{
public:
    ExtensionLoader(data::DataHolder* m_dataHolder, QMenuBar *menuBar, SettingsDialog* settingsDialog, QSystemTrayIcon* trayIcon);
    void loadAll(ExtensionBar* extensionBar, QSplashScreen *splashScreen, el::base::type::StoragePointer loggingStoragePointer) const;
    static bool verifyExtension(const QString& filename);
private:
    data::DataHolder* m_dataHolder;
    QMenuBar* m_menuBar;
    SettingsDialog* m_settingsDialog;
    QSystemTrayIcon* m_trayIcon;
};

#endif // EXTENSION_LOADER_H
