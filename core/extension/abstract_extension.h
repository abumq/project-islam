#ifndef ABSTRACT_EXTENSION_H
#define ABSTRACT_EXTENSION_H

#include <QWidget>
#include "core/logging.h"
#include "core/constants.h"
#include "core/data/data_holder.h"

class ExtensionInfo;
class ExtensionItem;
class QLabel;

class AbstractExtension : public QWidget
{
    Q_OBJECT
public:
    static const int kExtensionTop = 21;
    static const unsigned int kExtensionMaxLengthAuthor = 30;
    static const unsigned int kExtensionMaxLengthName = 12;
    static const unsigned int kExtensionMaxLengthTitle = 50;
    static const unsigned int kExtensionMaxLengthDescription = 1000;
    static const unsigned int kExtensionStartTop = 53;
    
    AbstractExtension(QWidget *parent, ExtensionInfo* info, bool isDefault = false);
    virtual ~AbstractExtension();
    
    bool operator==(const AbstractExtension& ex);
    
    const ExtensionInfo* info() const;
    
    bool isDefault(void) const;
    void setIsDefault(bool);
    
    QString htmlFormattedDescription() const;
    
    void setExtensionItem(ExtensionItem* extensionItem);
    ExtensionItem* extensionItem() const;
    
    QWidget* container();
    void setParent(QWidget *container);
    
    void activate();
    void deactivate();
    
    void update();
    
    data::DataHolder* dataHolder();
    void setDataHolder(data::DataHolder* dataHolder);
    
    QLabel* titleLabel() const;
signals:
    void containerGeometryChanged();
protected:
    void resizeEvent(QResizeEvent *);
private:
    QWidget* m_parent;
    QWidget* m_container;
    data::DataHolder* m_dataHolder;
    QLabel* m_titleLabel;
    ExtensionInfo* m_info;
    ExtensionItem* m_extensionItem;
    QString m_htmlFormattedDescription;
    bool m_isDefault;
    
    void buildHtmlFormattedDescription();
};
                    
#endif // ABSTRACT_EXTENSION_H
