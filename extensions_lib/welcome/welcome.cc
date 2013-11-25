#include "welcome.h"
#include <QLabel>

Welcome::Welcome(QWidget *parent, data::DataHolder* dataHolder) :
    Extension(parent, dataHolder, "Welcome", "Welcome Page",
        "A welcome page"
    )
{
}

Welcome::~Welcome()
{
}

void Welcome::initialize()
{
    new QLabel("Wow", container());
}
