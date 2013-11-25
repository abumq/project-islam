#include "core/extension/extension_loader.h"
#include <QPluginLoader>
#include <QFileDialog>
#include "core/logging.h"
#include "core/extension/extension_interface.h"
#include "core/extension/extension.h"
#include "core/extension/extension_bar.h"
#include "core/data/data_holder.h"

ExtensionLoader::ExtensionLoader(data::DataHolder* dataHolder) :
    m_dataHolder(dataHolder)
{
}

void ExtensionLoader::loadAll(ExtensionBar* extensionBar) const
{
    _TRACE;
    LOG(INFO) << "Loading all the extensions. ExtensionBar [" << extensionBar << "]";
    
    // TODO: Loop through extensions file and load each extension
    
    QPluginLoader loader("/home/majid/projects/build/extensions/libMemorization.so");
    ExtensionInterface* extensionInterface = qobject_cast<ExtensionInterface*>(loader.instance());
    if (extensionInterface != nullptr) {
        extensionInterface->extension()->setDataHolder(m_dataHolder);
        extensionInterface->extension()->setParent(extensionBar->container());
        // reinitialize
        extensionInterface->initialize();
        
        extensionBar->addExtension(extensionInterface->extension());
    } else {
        LOG(ERROR) << "Error occured while loading [" << loader.fileName() << "]: " << loader.errorString();
    }
}
