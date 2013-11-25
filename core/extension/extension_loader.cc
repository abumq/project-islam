#include "core/extension/extension_loader.h"
#include <QPluginLoader>
#include <QFileDialog>
#include "core/logging.h"
#include "core/extension/abstract_extension.h"
#include "core/extension/extension.h"
#include "core/extension/extension_bar.h"
#include "core/data/data_holder.h"

ExtensionLoader::ExtensionLoader(data::DataHolder* dataHolder) :
    m_dataHolder(dataHolder)
{
}

void ExtensionLoader::loadAll(const QString& appPath, ExtensionBar* extensionBar) const
{
    _TRACE;
    LOG(INFO) << "Loading all the extensions. ExtensionBar [" << extensionBar << "]; application path: "
                << appPath;
    
    QDir extensionsDir(appPath + "/extensions/", "*.so", QDir::Name | QDir::IgnoreCase, QDir::Files);
    
    for (QString extensionFilename : extensionsDir.entryList()) {
        QPluginLoader loader(extensionsDir.absoluteFilePath(extensionFilename));
        AbstractExtension* abstractExtension = qobject_cast<AbstractExtension*>(loader.instance());
        if (abstractExtension != nullptr) {
            abstractExtension->extension()->setDataHolder(m_dataHolder);
            abstractExtension->extension()->setParent(extensionBar->container());
            // initialize
            abstractExtension->initialize(el::Loggers::getLogger("default")->configurations());
            
            extensionBar->addExtension(abstractExtension->extension());
        } else {
            LOG(ERROR) << "Error occured while loading extension [" << loader.fileName() << "]: " << loader.errorString();
        }
    }
}
