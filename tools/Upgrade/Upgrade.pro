QT       += core
QT       -= gui

unix:!win32 {
    ## Compiler
    COMPILER = icpc

    ## Standard
    QMAKE_CXXFLAGS += -std=c++0x

    ## Warning suppressions
    QMAKE_CXXFLAGS += -Wno-sign-compare
    
    ## Other flags
    QMAKE_CXXFLAGS += -fPIC -g
    
    QMAKE_CC = $$COMPILER
    QMAKE_CXX = $$COMPILER
    QMAKE_LINK = $$COMPILER

    ## Unix specific defines
    DEFINES += _ELPP_STACKTRACE_ON_CRASH
}
DEFINES += _ELPP_QT_LOGGING
           
TARGET = upgrade-lc-now
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cc

HEADERS += \
    easylogging++.h
