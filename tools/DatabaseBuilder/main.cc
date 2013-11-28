#include "databasebuilder.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DatabaseBuilder w;
    w.show();
    
    return a.exec();
}
