#include "core/extension/abstract_extension.h"
#include <QLabel>
#include <QResizeEvent>
#include "core/logging.h"
#include "core/constants.h"
#include "core/extension/extension_item.h"
#include "core/extension/extension_bar.h"
#include "core/extension/extension_info.h"

AbstractExtension::AbstractExtension(QWidget *parent, ExtensionInfo *extensionInfo, bool isDefault) :
    QWidget(parent),
    m_parent(parent),
    m_dataHolder(nullptr),
    m_titleLabel(nullptr),
    m_info(extensionInfo),
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
}

AbstractExtension::~AbstractExtension()
{
    LOG(INFO) << "Unloading extension [" << info()->name() << "]";
    delete m_titleLabel;
    m_titleLabel = nullptr;
    delete m_container;
    m_container = nullptr;
}

bool AbstractExtension::operator ==(const AbstractExtension &ex)
{
    if (ex.info() == nullptr) {
        return info() == nullptr;
    } else if (info() == nullptr) {
        return false;
    }
    return ex.info()->name() == info()->name();
}

const ExtensionInfo *AbstractExtension::info() const
{
    return m_info;
}

bool AbstractExtension::isDefault() const
{
    return m_isDefault;
}

void AbstractExtension::setIsDefault(bool value)
{
    m_isDefault = value;
}

QString AbstractExtension::htmlFormattedDescription() const 
{
    return m_htmlFormattedDescription;
}

void AbstractExtension::setExtensionItem(ExtensionItem *extensionItem)
{
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

void AbstractExtension::update()
{
    QWidget::update();
    if (m_titleLabel != nullptr) {
        m_titleLabel->setMinimumSize(width(), m_titleLabel->height());
    }
    if (m_container != nullptr) {
        LOG(DEBUG) << "Resizing container to [" << width() << " x " << height() << "]";
        m_container->setMinimumSize(width(), height());
        m_container->setMaximumSize(width(), height());
        emit containerGeometryChanged();
    }
}

QWidget* AbstractExtension::container()
{
    return m_container;
}

void AbstractExtension::setParent(QWidget *parent)
{
    QWidget::setParent(parent);
    m_parent = parent;
    if (m_parent != nullptr) {
        resize(m_parent->width(), m_parent->height());
    }
}

void AbstractExtension::activate()
{
    m_extensionItem->select();
    show();
    m_extensionItem->extensionBar()->setCurrentExtension(this);
}

void AbstractExtension::deactivate()
{
    m_extensionItem->deselect();
    hide();
    m_extensionItem->extensionBar()->setCurrentExtension(nullptr);
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

void AbstractExtension::buildHtmlFormattedDescription()
{
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
