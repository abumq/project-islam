
NAME = AlQuran
include(../../extension.pri)

DEFINES += # User macros

SOURCES +=  \
    al_quran.cc \
    bookmark.cc \
    bookmarks_bar.cc
    
HEADERS += \
    al_quran.h \
    bookmark.h \
    bookmarks_bar.h

FORMS += \
    bookmarks_bar.ui

RESOURCES += \
    icons.qrc
