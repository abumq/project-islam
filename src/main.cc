#include <QApplication>
#include <QDebug>
#include "log/logging.hh"

#include "db/database_builder.hh"

#include "ui/mainwindow.hh"

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    _START_EASYLOGGINGPP(argc, argv);
    configureLoggers();
    
    
    // CHECK(db::DatabaseBuilder::build("../database/quran/quran_chapter.sql"));
    //CHECK(db::DatabaseBuilder::build("../database/quran/quran_english.sql"));
    // CHECK(db::DatabaseBuilder::build("../database/dua.sql"));
    //CHECK(db::DatabaseBuilder::build("../database/quran/quran_arabic.sql"));

    ui::MainWindow window;
    window.show();
    return a.exec();
}
