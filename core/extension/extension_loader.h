#ifndef EXTENSION_LOADER_H
#define EXTENSION_LOADER_H

#include <QString>

class ExtensionBar;
class QMenuBar;
namespace data {
class DataHolder;
}

class ExtensionLoader
{
public:
    explicit ExtensionLoader(data::DataHolder* m_dataHolder, QMenuBar *menuBar);
    void loadAll(const QString& appPath, ExtensionBar* extensionBar) const;
private:
    data::DataHolder* m_dataHolder;
    QMenuBar* m_menuBar;
};

#endif // EXTENSION_LOADER_H
