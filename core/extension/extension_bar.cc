#include "core/extension/extension_bar.h"

#include <QVector>

#include "core/logging.h"
#include "core/constants.h"
#include "core/memory.h"
#include "core/extension/abstract_extension.h"
#include "core/extension/extension_info.h"
#include "core/extension/extension_item.h"

ExtensionBar::ExtensionBar(QWidget *parent, QWidget* container) :
    QToolBar(parent),
    m_mainWindow(parent),
    m_container(container),
    m_currentExtension(nullptr)
{
    _TRACE;
    setMinimumWidth(kExtensionBarWidth);
    setMaximumWidth(kExtensionBarWidth);
    setFloatable(false);
    setOrientation(Qt::Vertical);
    setMovable(false);
}

ExtensionBar::~ExtensionBar()
{
    _TRACE;
    memory::clearPointerContainer(m_extensionItems.begin(), m_extensionItems.end(), &m_extensionItems);
    CHECK(m_extensionItems.empty()) << "Unable to retain memory from the extensions items. "
                                << "Memory leak from [" << m_extensionItems.size() << "] extensions items.";
    memory::clearPointerContainer(m_extensions.begin(), m_extensions.end(), &m_extensions);
    CHECK(m_extensions.empty()) << "Unable to retain memory from the extensions. "
                                << "Memory leak from [" << m_extensions.size() << "] extensions.";
}

void ExtensionBar::addExtension(AbstractExtension* extension)
{
    for (AbstractExtension* e : m_extensions) {
        // Ensure we dont have extension with same name - 
        // we dont have any other way excepting looping through manually since we have
        // container of extension pointer
        if (*e == *extension) {
            LOG(WARNING) << "Ignoring extension [" << extension->info()->name() << "], "
                            << "Already contains extension with same name."
                            << " Unloading extension...";
            // TODO: delete? Answer relies on how extension loader :: loadAll works
            // delete extension;
            return;
        }
    }
    if (extension->isDefault()) {
        // If we already have default, remove it
        for (ExtensionItem* extensionItem : m_extensionItems) {
            extensionItem->extension()->setIsDefault(false);
        }
    } else {
        // We use first extension as default
        if (m_extensionItems.isEmpty()) {
            extension->setIsDefault(true);
        }
    }
    m_extensions.push_back(extension);
    ExtensionItem* extensionItem = new ExtensionItem(this, extension, &m_extensionItems);
    extensionItem->setText(extension->info()->name());
    extensionItem->show();
    m_extensionItems.push_back(extensionItem);
    addWidget(extensionItem);
}

ExtensionItem* ExtensionBar::defaultExtensionItem() const
{
    for (ExtensionItem* extensionItem : m_extensionItems) {
        if (extensionItem->extension()->isDefault()) {
            return extensionItem;
        }
    }
    return nullptr;
}

void ExtensionBar::setCurrentExtension(AbstractExtension *currentExtension)
{
    m_currentExtension = currentExtension;
    emit extensionChanged(currentExtension);
}

AbstractExtension *ExtensionBar::currentExtension() const
{
    return m_currentExtension;
}

QWidget *ExtensionBar::container() const
{
    return m_container;
}

const QVector<ExtensionItem*>* ExtensionBar::extensionItems() const
{
    return &m_extensionItems;
}

const QVector<AbstractExtension*>* ExtensionBar::extensions() const
{
    return &m_extensions;
}

AbstractExtension* ExtensionBar::hasExtension(const QString& name) const
{
    for (AbstractExtension* e : m_extensions) {
        if (e->info()->name().toLower() == name.toLower()) {
            return e;
        }
    }
    return nullptr;
}
