#ifndef ABSTRACT_EXTENSION_H
#define ABSTRACT_EXTENSION_H

#include <QWidget>

#include "core/logging/logging.h"
#include "core/constants.h"
#include "core/data/data_holder.h"

class ExtensionInfo;
class ExtensionItem;
class QLabel;
class QMenu;
class SettingsLoader;
class ExtensionBase;
class QSystemTrayIcon;

class AbstractExtension : public QWidget
{
    Q_OBJECT
public:
    static const int kExtensionTop = 17;
    /*static const unsigned int kExtensionMaxLengthAuthor = 30;
    static const unsigned int kExtensionMaxLengthName = 12;
    static const unsigned int kExtensionMaxLengthTitle = 50;
    static const unsigned int kExtensionMaxLengthDescription = 1000;*/
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
    void setContainer(QWidget *container);
    
    void activate();
    void deactivate();
    
    void update();
    
    data::DataHolder* dataHolder();
    void setDataHolder(data::DataHolder* dataHolder);
    
    QLabel* titleLabel() const;
    
    QMenu* menu() const;
    QWidget* settingsTabWidget() const;
    
    void setSettingsLoader(SettingsLoader* settingsLoader);
    SettingsLoader* settingsLoader() const;
    
    void setSettingsMap(QMap<QString, QVariant>* map);
    QMap<QString, QVariant>* settingsMap() const;
    
    void setTrayIcon(QSystemTrayIcon* trayIcon);
    QSystemTrayIcon* trayIcon() const;
    
    int containerWidth() const;
    int containerHeight() const;
    
signals:
    void containerGeometryChanged(int, int);
    void activated();
    void deactivated();
protected:
    void resizeEvent(QResizeEvent *);
private:
    QWidget* m_parent;
    QWidget* m_container;
    data::DataHolder* m_dataHolder;
    QLabel* m_titleLabel;
    ExtensionInfo* m_info;
    ExtensionItem* m_extensionItem;
    SettingsLoader* m_settingsLoader;
    QMenu *m_menu;
    QMap<QString, QVariant>* m_settingsMap;
    QWidget* m_settingsTabWidget;
    QString m_htmlFormattedDescription;
    QSystemTrayIcon* m_trayIcon;
    bool m_isDefault;
    
    int m_containerWidth;
    int m_containerHeight;
    
    friend class ExtensionBase;
    
    void buildHtmlFormattedDescription();
    QWidget* newOrExistingSettingsTabWidget();
};

#endif // ABSTRACT_EXTENSION_H
