#ifndef EXTENSION_LOADER_H
#define EXTENSION_LOADER_H

#include <QString>

class ExtensionBar;
class SettingsLoader;
class QMenuBar;
namespace data {
class DataHolder;
}

class ExtensionLoader
{
public:
    ExtensionLoader(data::DataHolder* m_dataHolder, SettingsLoader* settingsLoader, QMenuBar *menuBar);
    void loadAll(const QString& appPath, ExtensionBar* extensionBar) const;
private:
    data::DataHolder* m_dataHolder;
    SettingsLoader* m_settingsLoader;
    QMenuBar* m_menuBar;
};

#endif // EXTENSION_LOADER_H
