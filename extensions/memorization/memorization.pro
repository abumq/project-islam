QT       += core gui widgets sql multimedia
 
TARGET = $$qtLibraryTarget(Memorization)
TEMPLATE = lib

QMAKE_CXXFLAGS += -std=c++0x -fPIC -g

COMPILER = g++
QMAKE_CC = $$COMPILER
QMAKE_CXX = $$COMPILER
QMAKE_LINK = $$COMPILER

DEFINES += _ELPP_UNICODE \
           _ELPP_QT_LOGGING \
           _ELPP_STL_LOGGING \
           _ELPP_DEFAULT_LOG_FILE=\\\"l.lg\\\"
           
INCLUDEPATH += "../../"

SOURCES +=  memorization.cc \
    ../../core/extension/extension.cc \
    ../../core/data/data_holder.cc \
    ../../core/controls/quran_view.cc \
    ../../core/controls/quran_reciter.cc \
    ../../core/controls/quran_reader.cc \
    ../../core/quran/verse.cc \
    ../../core/quran/quran.cc \
    ../../core/quran/chapter.cc \
    ../../core/settings_loader.cc

HEADERS += memorization.h \
            memorization_global.h \
    ../../core/extension/extension.h \
    ../../core/data/data_holder.h \
    ../../core/controls/quran_view.h \
    ../../core/controls/quran_reciter.h \
    ../../core/controls/quran_reader.h \
    ../../core/quran/verse.h \
    ../../core/quran/quran.h \
    ../../core/quran/chapter.h \
    ../../core/settings_loader.h

FORMS += \
    ../../core/controls/quran_reciter.ui \
    ../../core/controls/quran_reader.ui
