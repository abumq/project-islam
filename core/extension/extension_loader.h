#ifndef EXTENSION_LOADER_H
#define EXTENSION_LOADER_H

#include <QString>

class ExtensionBar;
class SettingsLoader;
class QMenuBar;
class QTabWidget;
class QSplashScreen;
namespace data {
class DataHolder;
}

class ExtensionLoader
{
public:
    ExtensionLoader(data::DataHolder* m_dataHolder, QMenuBar *menuBar, QTabWidget* tabWidget);
    void loadAll(ExtensionBar* extensionBar, QSplashScreen *splashScreen) const;
    static bool verifyExtension(const QString& filename);
private:
    data::DataHolder* m_dataHolder;
    QMenuBar* m_menuBar;
    QTabWidget* m_settingsTabWidget;
};

#endif // EXTENSION_LOADER_H
