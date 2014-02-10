#ifndef EXTENSION_ITEM_H
#define EXTENSION_ITEM_H

#include <QToolButton>

class AbstractExtension;
class ExtensionBar;
template <class T> class QVector;

class ExtensionItem : public QToolButton
{
    Q_OBJECT
public:
    static const int kExtensionItemWidth = 82;
    static const int kExtensionItemHeight = 55;
    static const int kExtensionItemFontSize = 9;
    
    ExtensionItem(ExtensionBar *extensionBar,  AbstractExtension *extension,  QVector<ExtensionItem*>* allExtensionItems);
    ~ExtensionItem();
    
    AbstractExtension* extension(void) const;
    
    void select(void);
    void deselect(void);
    
    ExtensionBar* extensionBar() const;
    
    QString iconName() const;
public slots:
    void showExtension(bool);
private:
    ExtensionBar* m_extensionBar;
    AbstractExtension* m_extension;
    QVector<ExtensionItem*>* m_allExtensionItems;
    QString m_icon;
};

#endif // EXTENSION_ITEM_H
