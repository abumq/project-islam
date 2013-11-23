#include "core/extension/extension_loader.h"
#include "core/logging.h"
#include "core/extension/extension_bar.h"
#include "core/data/data_holder.h"
#include "extensions/memorization.h"

ExtensionLoader::ExtensionLoader(data::DataHolder* dataHolder) :
    m_dataHolder(dataHolder)
{
}

void ExtensionLoader::loadAll(ExtensionBar* extensionBar) const
{
    _TRACE;
    LOG(INFO) << "Loading all the extensions. ExtensionBar [" << extensionBar << "]";
    
    extensionBar->addExtension(new Memorization(extensionBar->container(), m_dataHolder));
}
