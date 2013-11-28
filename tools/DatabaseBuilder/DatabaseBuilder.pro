QT       += widgets core gui sql

greaterThan(QT_MAJOR_VERSION, 4): 

TARGET = DatabaseBuilder
TEMPLATE = app


SOURCES += main.cc\
        databasebuilder.cc

HEADERS  += databasebuilder.h

FORMS    += databasebuilder.ui

QMAKE_CXXFLAGS += -std=c++0x -fPIE

COMPILER = g++
QMAKE_CC = $$COMPILER
QMAKE_CXX = $$COMPILER
QMAKE_LINK = $$COMPILER
