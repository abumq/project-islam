#include "extension.h"
#include <QLabel>
#include "main_area.h"
namespace core {
Extension::Extension(const QString& name, QWidget *parent) :
    QWidget(parent),
    m_name(name)
{
    //parent->findChild<QLabel*>("lblTitle")->setText(name);
}

Extension::~Extension()
{
}

const QString& Extension::name() const
{
    return m_name;
}
}
