#ifndef EXTENSION_LOADER_H
#define EXTENSION_LOADER_H

#include <QString>

class ExtensionBar;
class SettingsLoader;
class QApplication;
class QMenuBar;
namespace data {
class DataHolder;
}

class ExtensionLoader
{
public:
    ExtensionLoader(data::DataHolder* m_dataHolder, SettingsLoader* settingsLoader, 
                    QApplication *app, QMenuBar *menuBar);
    void loadAll(const QString& appPath, ExtensionBar* extensionBar) const;
    static bool verifyExtension(const QString& filename);
private:
    data::DataHolder* m_dataHolder;
    SettingsLoader* m_settingsLoader;
    QApplication* m_app;
    QMenuBar* m_menuBar;
};

#endif // EXTENSION_LOADER_H
