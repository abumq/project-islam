QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectIslam
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

DEFINES += _ELPP_UNICODE \
           _ELPP_QT_LOGGING \
            _ELPP_STL_LOGGING \
            _ELPP_LOG_UNORDERED_MAP \
            _ELPP_NO_DEFAULT_LOG_FILE \
            _ELPP_STACKTRACE_ON_CRASH
            
COMPILER = g++
QMAKE_CC = $$COMPILER
QMAKE_CXX = $$COMPILER
QMAKE_LINK = $$COMPILER

SOURCES += \
    main.cc \
    db/database_manager.cc \
    db/database_builder.cc \
    ui/mainwindow.cc \
    ui/sidebar_widget.cc \
    quran/verse.cc \
    quran/chapter.cc \
    quran/quran.cc \
    quran/quran_widget.cc \
    quran/quran_reader.cc

HEADERS  += \
    log/easylogging++.h \
    log/logging.hh \
    db/database_manager.hh \
    db/database_builder.hh \
    ui/mainwindow.hh \
    ui/sidebar_widget.hh \
    quran/verse.hh \
    quran/quran.hh \
    quran/chapter.hh \
    quran/quran_widget.hh \
    quran/quran_widget.hh \
    quran/quran_reader.hh

FORMS    += \
    ui/forms/mainwindow.ui \
    ui/forms/sidebar_widget.ui \
    quran/forms/quran_widget.ui

OTHER_FILES += \
    ../config/logging.conf \
    ../database/quran/quran_chapter.sql \
    ../database/dua.sql \
    ../database/quran/quran_english.sql \
    ../rc/icons.rc \
    ../database/quran/quran_arabic.sql
