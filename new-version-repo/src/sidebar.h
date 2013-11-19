#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QVector>
#include "extension.h"

namespace Ui {
class Sidebar;
}

class Sidebar : public QWidget
{
    Q_OBJECT
    
public:
    explicit Sidebar(QWidget *parent, QWidget *mainArea);
    ~Sidebar();
    
    const QVector<core::Extension*>& extensions(void) const;
    void addExtension(core::Extension* extension);
    void loadExtensions(QWidget* mainArea);
    void showExtensions();
private:
    Ui::Sidebar *ui;
    QVector<core::Extension*> m_extensions;
};

#endif // SIDEBAR_H
