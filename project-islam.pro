QT       += core gui widgets sql multimedia

greaterThan(QT_MAJOR_VERSION, 4):

TARGET = project-islam
TEMPLATE = app


QMAKE_CXXFLAGS += -std=c++0x -fPIE -g

COMPILER = g++
QMAKE_CC = $$COMPILER
QMAKE_CXX = $$COMPILER
QMAKE_LINK = $$COMPILER

DEFINES += _ELPP_UNICODE \
           _ELPP_QT_LOGGING \
           _ELPP_STL_LOGGING \
           _ELPP_LOG_UNORDERED_MAP \
           _ELPP_NO_DEFAULT_LOG_FILE \
           _ELPP_STACKTRACE_ON_CRASH

#LIBS += -Lextensions

# Extensions
#LIBS += -lmemorization

# libimf for Intel C++
# TODO: export LD_LIBRARY_PATH
LIBS += -L/home/mkhan/intel/composer_xe_2013.5.192/compiler/lib/intel64/
 
SOURCES += main.cc\
        mainwindow.cc \
    core/style_loader.cc \
    core/extension/extension_loader.cc \
    core/extension/extension_bar.cc \
    core/extension/extension_item.cc \
    core/extension/extension.cc \
    core/controls/colorbox.cc \
    core/settings_loader.cc \
    settings_dialog.cc \
    core/quran/verse.cc \
    core/quran/quran.cc \
    core/quran/chapter.cc \
    core/data/database_manager.cc \
    core/data/database_builder.cc \
    core/data/data_holder.cc \
    core/controls/quran_reciter.cc \
    core/controls/quran_reader.cc \
    core/controls/quran_view.cc

HEADERS  += mainwindow.h \
    core/constants.h \
    core/style_loader.h \
    core/settings_loader.h \
    core/logging.h \
    core/extension/extension_bar.h \
    core/extension/extension_item.h \
    core/extension/extension.h \
    core/extension/extension_interface.h \
    core/extension/extension_loader.h \
    core/controls/colorbox.h \
    settings_dialog.h \
    core/quran/chapter.h \
    core/quran/quran.h \
    core/quran/verse.h \
    core/data/database_manager.h \
    core/data/database_builder.h \
    core/data/database_manager.h \
    core/data/database_builder.h \
    core/data/data_holder.h \
    core/controls/quran_reciter.h \
    core/controls/quran_reader.h \
    core/controls/quran_view.h

FORMS    += mainwindow.ui \
    settings_dialog.ui \
    core/controls/quran_reciter.ui \
    core/controls/quran_reader.ui

OTHER_FILES += \
    resources/styles/menu.style \
    resources/styles/extension_bar.style \
    resources/styles/extension.style \
    resources/styles/extension_item.style \
    resources/images/brain.png \
    resources/data/sql/dua.sql \
    resources/data/sql/quran/quran_english.sql \
    resources/data/sql/quran/quran_chapter.sql \
    resources/data/sql/quran/quran_arabic.sql \
    resources/configs/logging.linux.config \
    resources/configs/logging.win.config

RESOURCES += \
    resources/styles.qrc \
    resources/icons.qrc \
    resources/sql_data.qrc
