#ifndef EXTENSION_BAR_H
#define EXTENSION_BAR_H

#include <QToolBar>

class AbstractExtension;
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
    void addExtension(AbstractExtension* extension);
    ExtensionItem* defaultExtensionItem(void) const;
    
    void setCurrentExtension(AbstractExtension* currentExtension);
    AbstractExtension* currentExtension() const;
    
    QWidget* container(void) const;
    
    const QVector<ExtensionItem*>* extensionItems() const;
    const QVector<AbstractExtension*>* extensions() const;
    AbstractExtension* hasExtension(const QString& name) const;
signals:
    void extensionChanged(AbstractExtension* extension);
private:
    QWidget* m_mainWindow;
    QWidget* m_container;
    AbstractExtension* m_currentExtension;
    QVector<AbstractExtension*> m_extensions;
    QVector<ExtensionItem*> m_extensionItems;
};

#endif // EXTENSION_BAR_H
