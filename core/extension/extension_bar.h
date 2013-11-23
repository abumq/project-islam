#ifndef EXTENSION_BAR_H
#define EXTENSION_BAR_H

#include <QToolBar>

class Extension;
class ExtensionItem;

template <class T> class QVector;

class ExtensionBar : public QToolBar
{
    Q_OBJECT
    
public:
    
    static const int kExtensionBarWidth = 82;
    
    ExtensionBar(QWidget *parent, QWidget* container);
    ~ExtensionBar();
    QWidget* mainFrame();
    void addExtension(Extension* extension);
    ExtensionItem* defaultExtensionItem(void) const;
    
    void setCurrentExtension(Extension* currentExtension);
    Extension* currentExtension() const;
    
    QWidget* container(void) const;
    
    const QVector<ExtensionItem*>* extensionItems() const;
    const QVector<Extension*>* extensions() const;
signals:
    void extensionChanged(Extension* extension);
private:
    QWidget* m_mainWindow;
    QWidget* m_container;
    Extension* m_currentExtension;
    QVector<Extension*> m_extensions;
    QVector<ExtensionItem*> m_extensionItems;
};

#endif // EXTENSION_BAR_H
