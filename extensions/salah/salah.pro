
NAME = Salah
include(../../extension.pri)

DEFINES += # User macros

SOURCES += \
    qibla_compass.cc \
    salah_clock.cc \
    salah.cc \
    salah_times.cc \
    settings_tab_widget_form.cc \
    ../../core/controls/clock.cc

HEADERS += \
    qibla_compass.h \
    salah_clock.h \
    salah.h \
    salah_times.h \
    settings_tab_widget_form.h \
    ../../core/controls/clock.h

FORMS += \
    settings_tab_widget_form.ui

RESOURCES += \
    icons_salah.qrc
