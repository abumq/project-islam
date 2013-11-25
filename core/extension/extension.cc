#include "core/extension/extension.h"
#include <QLabel>
#include <QResizeEvent>
#include "core/logging.h"
#include "core/constants.h"
#include "core/extension/extension_item.h"
#include "core/extension/extension_bar.h"

Extension::Extension(QWidget *parent, data::DataHolder* dataHolder, const QString& name,
        const QString &title, const QString &description, bool isDefault) :
    QWidget(parent),
    m_parent(parent),
    m_dataHolder(dataHolder),
    m_titleLabel(nullptr),
    m_name(name),
    m_title(title),
    m_description(description),
    m_isDefault(isDefault)
{
    _TRACE;
    LOG(INFO) << "Loading extension [" << name << "]";
    // Name length check
    CHECK(!m_name.isEmpty()) << "Extension should have a name";
    CHECK(m_name.length() <= kExtensionMaxLengthName) << "Extension name should be <= " << kExtensionMaxLengthName 
                                                      << ", got [" << m_name.size() << "] for [" << m_name << "]";
    
    // Title length check
    if (m_title.isEmpty()) {
        LOG(WARNING) << "No title for [" << m_name << "], using name as title";
        m_title = m_name;
    } else {
        CHECK(m_title.length() <= kExtensionMaxLengthTitle) << "Extension title should be <= " << kExtensionMaxLengthTitle 
                                                            << ", got [" << m_title.size() << "] for [" << m_name << "]";
    }
    
    // Description length check
    CHECK(m_description.length() <= kExtensionMaxLengthDescription) << "Extension description should be <= " << kExtensionMaxLengthDescription 
                                                                    << ", got [" << m_description.size() << "] for [" << m_name << "]";
    hide();
    resize(parent->width(), parent->height());
    
    m_titleLabel = new QLabel(this);
    m_titleLabel->setObjectName("extensionTitle");
    buildHtmlFormattedDescription();
    m_container = new QWidget(this);
    m_container->move(0, kExtensionStartTop);
}

Extension::~Extension()
{
    _TRACE;
    LOG(INFO) << "Unloading extension [" << m_name << "]";
    delete m_titleLabel;
    m_titleLabel = nullptr;
    delete m_container;
    m_container = nullptr;
}

QString Extension::name() const
{
    return m_name;
}

QString Extension::title() const
{
    return m_title;
}

QString Extension::description() const
{
    return m_description;
}

bool Extension::isDefault() const
{
    return m_isDefault;
}

void Extension::setIsDefault(bool value)
{
    m_isDefault = value;
}

QString Extension::htmlFormattedDescription() const 
{
    return m_htmlFormattedDescription;
}

void Extension::setExtensionItem(ExtensionItem *extensionItem)
{
    m_extensionItem = extensionItem;
    if (extensionItem != nullptr) {
        m_titleLabel->setText("<h1><img src='" + extensionItem->icon() + "'/>" + m_title + "</h1>");
    }
}

ExtensionItem *Extension::extensionItem() const
{
    return m_extensionItem;
}

void Extension::resizeEvent(QResizeEvent* event)
{
    event->accept();
    update();
}

void Extension::update()
{
    QWidget::update();
    if (m_titleLabel != nullptr) {
        m_titleLabel->setMinimumSize(width(), m_titleLabel->height());
    }
    if (m_container != nullptr) {
        LOG(DEBUG) << "Resizing container to [" << width() << " x " << height() << "]";
        m_container->setMinimumSize(width(), height());
        m_container->setMaximumSize(width(), height());
    }
}

QWidget* Extension::container()
{
    return m_container;
}

void Extension::activate()
{
    m_extensionItem->select();
    show();
    m_extensionItem->extensionBar()->setCurrentExtension(this);
}

void Extension::deactivate()
{
    m_extensionItem->deselect();
    hide();
    m_extensionItem->extensionBar()->setCurrentExtension(nullptr);
}

data::DataHolder* Extension::dataHolder()
{
    return m_dataHolder;
}

QLabel* Extension::titleLabel() const
{
    return m_titleLabel;
}

void Extension::buildHtmlFormattedDescription()
{
    if (m_description.isEmpty()) {
        if (m_title != m_name) {
            m_htmlFormattedDescription.append("<b>" + m_title + "</b>");
            m_htmlFormattedDescription.append(" (" + m_name + ")");
        }
    } else {
        m_htmlFormattedDescription.append("<b>" + m_title + "</b>");
        if (m_title != m_name) {
            m_htmlFormattedDescription.append(" (" + m_name + ")");
        }
        m_htmlFormattedDescription.append("<br/>");
        m_htmlFormattedDescription.append(m_description);
    }
}
