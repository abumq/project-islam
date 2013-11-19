#ifndef EXTENSION_H
#define EXTENSION_H

#include <QWidget>

namespace core {
class Extension : public QWidget
{
    Q_OBJECT
    
public:
    explicit Extension(const QString& name = "", QWidget *parent = 0);
    ~Extension();
    const QString& name(void) const;
    
private:
    QString m_name;
};
}
#endif // EXTENSION_H
