#include "core/extension/abstract_extension.h"

#include <QLabel>
#include <QResizeEvent>
#include <QMenuBar>

#include "core/logging/logging.h"
#include "core/constants.h"
#include "core/utils/memory.h"
#include "core/extension/extension_item.h"
#include "core/extension/extension_bar.h"
#include "core/extension/extension_info.h"

AbstractExtension::AbstractExtension(QWidget *parent, ExtensionInfo *extensionInfo, bool isDefault) :
    QWidget(parent),
    m_parent(parent),
    m_dataHolder(nullptr),
    m_titleLabel(nullptr),
    m_info(extensionInfo),
    m_settingsLoader(nullptr),
    m_menu(nullptr),
    m_settingsTabWidget(nullptr),
    m_isDefault(isDefault)
{
    _TRACE;
    LOG(INFO) << "Loading extension [" << extensionInfo << "]";
    
    hide();
    
    if (m_parent != nullptr) {
        resize(m_parent->width(), m_parent->height());
    }
    
    m_titleLabel = new QLabel(this);
    m_titleLabel->setObjectName("extensionTitle");
    buildHtmlFormattedDescription();
    m_container = new QWidget(this);
    m_container->move(0, kExtensionStartTop);
    m_titleLabel->setGeometry(0, 0, m_container->width(), kExtensionStartTop);
    m_menu = new QMenu(extensionInfo->name());
    m_menu->menuAction()->setVisible(false);
}

AbstractExtension::~AbstractExtension()
{
    LOG(INFO) << "Unloading extension [" << info()->name() << "]";
    memory::deleteAll(m_titleLabel, m_menu, m_settingsTabWidget, m_container);
}

bool AbstractExtension::operator ==(const AbstractExtension& ex)
{
    if (ex.info() == nullptr) {
        return info() == nullptr;
    } else if (info() == nullptr) {
        return false;
    }
    return ex.info()->name() == info()->name();
}

const ExtensionInfo* AbstractExtension::info() const
{
    return m_info;
}

bool AbstractExtension::isDefault() const
{
    return m_isDefault;
}

void AbstractExtension::setIsDefault(bool value)
{
    _TRACE;
    m_isDefault = value;
}

QString AbstractExtension::htmlFormattedDescription() const 
{
    return m_htmlFormattedDescription;
}

void AbstractExtension::setExtensionItem(ExtensionItem *extensionItem)
{
    _TRACE;
    m_extensionItem = extensionItem;
    if (extensionItem != nullptr) {
        m_titleLabel->setText("<h1><img src='" + extensionItem->icon() + "'/>" + info()->title() + "</h1>");
    }
}

ExtensionItem *AbstractExtension::extensionItem() const
{
    return m_extensionItem;
}

void AbstractExtension::resizeEvent(QResizeEvent* event)
{
    event->accept();
    update();
}
int AbstractExtension::containerHeight() const
{
    return m_containerHeight;
}

int AbstractExtension::containerWidth() const
{
    return m_containerWidth;
}

void AbstractExtension::update()
{
    QWidget::update();
    int w = width() - ExtensionBar::kExtensionBarWidth - 5 /* Margin for extension bar (from stylesheet) */;
    int h = height() - AbstractExtension::kExtensionStartTop - kExtensionTop;
    if (m_titleLabel != nullptr) {
        m_titleLabel->setMinimumSize(w, m_titleLabel->height());
    }
    if (m_container != nullptr) {
        LOG(DEBUG) << "Resizing container to [" << w << " x " << h << "]";
        m_container->setMinimumSize(w, h);
        m_container->setMaximumSize(w, h);
        m_containerWidth = w;
        m_containerHeight = h;
        emit containerGeometryChanged(w, h);
    }
}

QWidget* AbstractExtension::container()
{
    _TRACE;
    return m_container;
}

void AbstractExtension::setContainer(QWidget *parent)
{
    _TRACE;
    QWidget::setParent(parent);
    m_parent = parent;
    if (m_parent != nullptr) {
        resize(m_parent->width(), m_parent->height());
    }
}

void AbstractExtension::activate()
{
    _TRACE;
    m_extensionItem->select();
    show();
    m_extensionItem->extensionBar()->setCurrentExtension(this);
    m_menu->menuAction()->setVisible(!m_menu->actions().isEmpty());
    emit activated();
}

void AbstractExtension::deactivate()
{
    _TRACE;
    m_extensionItem->deselect();
    hide();
    m_extensionItem->extensionBar()->setCurrentExtension(nullptr);
    m_menu->menuAction()->setVisible(false);
    emit deactivated();
}

data::DataHolder* AbstractExtension::dataHolder()
{
    return m_dataHolder;
}

void AbstractExtension::setDataHolder(data::DataHolder *dataHolder)
{
    _TRACE;
    m_dataHolder = dataHolder;
}

QLabel* AbstractExtension::titleLabel() const
{
    return m_titleLabel;
}

QMenu* AbstractExtension::menu() const
{
    return m_menu;
}

QWidget* AbstractExtension::settingsTabWidget() const
{
    return m_settingsTabWidget;
}

void AbstractExtension::setSettingsLoader(SettingsLoader* settingsLoader)
{
    _TRACE;
    m_settingsLoader = settingsLoader;
}

SettingsLoader* AbstractExtension::settingsLoader() const
{
    return m_settingsLoader;
}

void AbstractExtension::buildHtmlFormattedDescription()
{
    _TRACE;
    if (info()->description().isEmpty()) {
        if (info()->title() != info()->name()) {
            m_htmlFormattedDescription.append("<b>" + info()->title() + "</b>");
            m_htmlFormattedDescription.append(" (" + info()->name() + ")");
        }
    } else {
        m_htmlFormattedDescription.append("<b>" + info()->title() + "</b>");
        if (info()->title() != info()->name()) {
            m_htmlFormattedDescription.append(" (" + info()->name() + ")");
        }
        m_htmlFormattedDescription.append("<br/>");
        m_htmlFormattedDescription.append(info()->description());
    }
}

QWidget* AbstractExtension::newOrExistingSettingsTabWidget()
{
    if (m_settingsTabWidget == nullptr) {
        m_settingsTabWidget = new QWidget;
    }
    return m_settingsTabWidget;
}
