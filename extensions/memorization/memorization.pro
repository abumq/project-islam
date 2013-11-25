QT       += gui widgets multimedia
 
TARGET = memorization
TEMPLATE = lib


QMAKE_CXXFLAGS += -std=c++0x -fPIC

# Assuming this extension is in extensions/ dir
INCLUDEPATH += "../../../project-islam"

SOURCES += memorization.cc

HEADERS += memorization.h \
            memorization_global.h
