#ifndef EXTENSION_H
#define EXTENSION_H

#include <QWidget>
#include <QSize>
#include "core/logging.h"
#include "core/constants.h"
#include "core/data/data_holder.h"

#include <QtCore/qglobal.h>

#if defined(EXTENSION_LIBRARY)
#  define EXTENSION_EXPORT Q_DECL_EXPORT
#else
#  define EXTENSION_EXPORT Q_DECL_IMPORT
#endif

class ExtensionItem;
class QLabel;

class Extension : public QWidget
{
public:
    static const int kExtensionTop = 21;
    static const int kExtensionMaxLengthName = 12;
    static const int kExtensionMaxLengthTitle = 50;
    static const int kExtensionMaxLengthDescription = 1000;
    static const int kExtensionStartTop = 53;
    
    Extension(QWidget *parent, data::DataHolder* m_dataHolder, const QString &name,
              const QString &title = "", const QString &description = "", bool isDefault = false);
    virtual ~Extension();
    
    virtual void initialize(void) = 0;
    
    QString name() const;
    QString title() const;
    QString description() const;
    
    bool isDefault(void) const;
    void setIsDefault(bool);
    
    QString htmlFormattedDescription() const;
    
    void setExtensionItem(ExtensionItem* extensionItem);
    ExtensionItem* extensionItem() const;
    
    void update(const QSize&);
    QWidget* container();
    
    void activate();
    void deactivate();
    
    data::DataHolder* dataHolder();
    
    QLabel* titleLabel() const;
protected:
    void resizeEvent(QResizeEvent *);
private:
    QWidget* m_parent;
    QWidget* m_container;
    data::DataHolder* m_dataHolder;
    QLabel* m_titleLabel;
    QString m_name;
    QString m_title;
    QString m_description;
    ExtensionItem* m_extensionItem;
    QString m_htmlFormattedDescription;
    bool m_isDefault;
    
    void buildHtmlFormattedDescription();
};

#endif // EXTENSION_H
