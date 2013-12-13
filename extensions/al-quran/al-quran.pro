
NAME = AlQuran
include(../../extension.pri)

DEFINES += # User macros

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

RESOURCES += \
    icons.qrc
