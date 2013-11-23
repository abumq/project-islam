#include "core/extension/extension_item.h"

#include <QToolButton>
#include <QVector>
#include <QPixmap>
#include <QSize>

#include "core/constants.h"
#include "core/extension/extension.h"
#include "core/extension/extension_bar.h"

ExtensionItem::ExtensionItem(ExtensionBar *extensionBar, Extension* extension, 
        QVector<ExtensionItem*>* allExtensionItems) :
    QToolButton(extensionBar),
    m_extensionBar(extensionBar),
    m_extension(extension),
    m_allExtensionItems(allExtensionItems)
{
    _TRACE;
    QFont extensionFont(font());
    extensionFont.setBold(true);
    extensionFont.setPixelSize(kExtensionItemFontSize);
    setFont(extensionFont);
    setCheckable(true);
    setMinimumSize(kExtensionItemWidth, kExtensionItemHeight);
    setMaximumSize(kExtensionItemWidth, kExtensionItemHeight);
    setAutoRaise(true);
    
    // Signals
    connect(this, SIGNAL(clicked(bool)), this, SLOT(showExtension(bool)));
    
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    
    m_icon = ":/extensions/" + m_extension->name().toLower();
    setIcon(QPixmap(m_icon));
    setIconSize(QSize(32, 32));
    
    if (!m_extension->htmlFormattedDescription().isEmpty()) {
        setToolTip(m_extension->htmlFormattedDescription());
    }
    
    extension->setExtensionItem(this);
    
    if (extension->isDefault()) {
        showExtension(true);
    }
}

ExtensionItem::~ExtensionItem()
{
    m_extension->setExtensionItem(nullptr);
}

Extension* ExtensionItem::extension() const
{
    return m_extension;
}

void ExtensionItem::select()
{
    setChecked(true);
}

void ExtensionItem::deselect()
{
    setChecked(false);
}

ExtensionBar *ExtensionItem::extensionBar() const
{
    return m_extensionBar;
}

QString ExtensionItem::icon() const
{
    return m_icon;
}

void ExtensionItem::showExtension(bool checked)
{
    _TRACE;
    if (!checked) {
        // Ignore unchecked of same extension
        m_extension->activate();
    } else {
        for (ExtensionItem* extensionItem : *m_allExtensionItems) {
            if (extensionItem != this) {
                extensionItem->extension()->hide();
                extensionItem->setChecked(false);
            }
        }
        m_extension->activate();
    }
}
