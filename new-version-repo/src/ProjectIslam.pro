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
            _ELPP_STACKTRACE_ON_CRASH \
            _ELPP_STOP_ON_FIRST_ASSERTION
            
COMPILER = g++
QMAKE_CC = $$COMPILER
QMAKE_CXX = $$COMPILER
QMAKE_LINK = $$COMPILER

SOURCES += \
    mainwindow.cc \
    main.cc \
    sidebar.cc \
    extension.cc \
    main_area.cc \
    features/welcome_page.cc

HEADERS  += mainwindow.h \
    sidebar.h \
    easylogging++.h \
    extension.h \
    features/welcome_page.h \
    main_area.h

FORMS    += mainwindow.ui \
    sidebar.ui \
    main_area.ui \
    features/welcome_page.ui
