#ifndef EXTENSION_LOADER_H
#define EXTENSION_LOADER_H

class ExtensionBar;
namespace data {
class DataHolder;
}

class ExtensionLoader
{
public:
    explicit ExtensionLoader(data::DataHolder* m_dataHolder);
    void loadAll(ExtensionBar* extensionBar) const;
private:
    data::DataHolder* m_dataHolder;
};

#endif // EXTENSION_LOADER_H
