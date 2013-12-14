#include "databasebuilder.h"
#include <QApplication>
#include <easylogging++.h>

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DatabaseBuilder w;
    w.show();
    
    return a.exec();
}
