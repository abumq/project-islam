QT       += widgets core gui sql

greaterThan(QT_MAJOR_VERSION, 4): 

TARGET = DatabaseBuilder
TEMPLATE = app

DEFINES += _ELPP_QT_LOGGING

COMPILER = icpc

SOURCES += main.cc\
        databasebuilder.cc \
    dataconverter.cc \
    databuilder.cc

HEADERS  += databasebuilder.h \
    dataconverter.h \
    databuilder.h

FORMS    += databasebuilder.ui

QMAKE_CXXFLAGS += -std=c++0x -fPIE

QMAKE_CC = $$COMPILER
QMAKE_CXX = $$COMPILER
QMAKE_LINK = $$COMPILER
