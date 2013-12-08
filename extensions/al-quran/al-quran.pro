QT       += core gui widgets sql multimedia
 
TARGET = $$qtLibraryTarget(AlQuran)
TEMPLATE = lib

## Standard
QMAKE_CXXFLAGS += -std=c++0x

## Warning suppressions
QMAKE_CXXFLAGS += -Wno-sign-compare

## Other flags
QMAKE_CXXFLAGS += -fPIC -g

COMPILER = g++
QMAKE_CC = $$COMPILER
QMAKE_CXX = $$COMPILER
QMAKE_LINK = $$COMPILER

DEFINES += _ELPP_UNICODE \
           _ELPP_QT_LOGGING \
           _ELPP_STL_LOGGING \
           _ELPP_DEFAULT_LOG_FILE=\\\".l.lg\\\" \
           _ELPP_STACKTRACE_ON_CRASH
           
INCLUDEPATH += "../../"

# libimf so for Intel C++
# TODO: export LD_LIBRARY_PATH
LIBS += -L/home/mkhan/intel/composer_xe_2013.5.192/compiler/lib/intel64/

SOURCES +=  \
    al_quran.cc \
    ../../core/extension/abstract_extension.cc \
    ../../core/data/data_holder.cc \
    ../../core/controls/quran_view.cc \
    ../../core/controls/quran_reciter.cc \
    ../../core/controls/quran_reader.cc \
    ../../core/quran/verse.cc \
    ../../core/quran/quran.cc \
    ../../core/quran/chapter.cc \
    ../../core/settings_loader.cc \
    bookmark.cc \
    bookmarks_bar.cc

HEADERS += \
    al_quran.h \
    ../../core/extension/abstract_extension.h \
    ../../core/data/data_holder.h \
    ../../core/controls/quran_view.h \
    ../../core/controls/quran_reciter.h \
    ../../core/controls/quran_reader.h \
    ../../core/quran/verse.h \
    ../../core/quran/quran.h \
    ../../core/quran/chapter.h \
    ../../core/settings_loader.h \
    bookmark.h \
    bookmarks_bar.h

FORMS += \
    ../../core/controls/quran_reciter.ui \
    ../../core/controls/quran_reader.ui \
    bookmarks_bar.ui
