#-------------------------------------------------
#
# Project created by QtCreator 2013-11-25T11:47:41
#
#-------------------------------------------------

QT       += gui
QT += widgets
 
TARGET = welcome
TEMPLATE = lib


QMAKE_CXXFLAGS += -std=c++0x -fPIC


DEFINES += EXTENSION_LIBRARY

INCLUDEPATH += "/home/mkhan/Dropbox/project-islam"

SOURCES += welcome.cc

HEADERS += welcome.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
